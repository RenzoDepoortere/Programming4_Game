#include "GridComponent.h"
#include "Renderer.h"
#include "GameObject.h"
#include "RenderTextureComponent.h"
#include "ResourceManager.h"
#include "RockComponent.h"

#include "rapidjson.h"
#include "istreamwrapper.h"
#include "document.h"

#include <iostream>
#include <SDL.h>

using namespace grid;

GridComponent::GridComponent(dae::GameObject* pParentObject)
	: Component{ pParentObject }
{
	// Defaults
	m_NrRows = 13;
	m_NrCols = 12;

	m_CellWidth = 40;
	m_CellHeight = 40;

	// Init grid
	InitGridCells();

	// Get components
	m_pRenderer = pParentObject->GetComponent<dae::RenderTextureComponent>();
	if (m_pRenderer == nullptr) std::cout << "Warning: the grid component needs a renderTextureComponent!" << std::endl;
}

Cell* GridComponent::GetCell(int index) const
{
	if (0 <= index && index < m_pCells.size())
	{
		return m_pCells[index].get();
	}
	else
	{
		//std::cout << "Error: Tried to get cell with invalid index" << std::endl;
		return nullptr;
	}
}
Cell* GridComponent::GetCell(const glm::vec3& worldPos) const
{
	return GetCell(worldPos.x, worldPos.y, worldPos.z);
}
Cell* GridComponent::GetCell(float x, float y, float) const
{
	const int gridWidth{ m_NrCols * m_CellWidth };
	const int gridHeight{ m_NrRows * m_CellHeight };

	if (gridWidth < x || gridHeight < y)
	{
		//std::cout << "Error: Tried to get cell with invalid worldPos" << std::endl;
		return {};
	}
	else
	{
		const int colIdx{ static_cast<int>(x / m_CellWidth) };
		const int rowIdx{ static_cast<int>(y / m_CellHeight) };

		const int cellIdx{ rowIdx * m_NrCols + colIdx };
		return GetCell(cellIdx);
	}
}

void GridComponent::Render() const
{
	RenderGrid();
	//RenderDebugGrid();
}

void GridComponent::AddConnections(Cell* pCell)
{
	const int row{ static_cast<int>(pCell->rowCol.x) };
	const int col{ static_cast<int>(pCell->rowCol.y) };

	// Left cell
	int gridIdx = row * m_NrCols + (col - 1);
	CheckNeighborCell(pCell, gridIdx, true);

	// Right cell
	gridIdx = row * m_NrCols + (col + 1);
	CheckNeighborCell(pCell, gridIdx, true);

	// Below cell
	gridIdx = (row + 1) * m_NrCols + col;
	CheckNeighborCell(pCell, gridIdx, false);

	// Above cell
	gridIdx = (row - 1) * m_NrCols + col;
	CheckNeighborCell(pCell, gridIdx, false);
}
void GridComponent::RemoveConnections(Cell* pCell)
{
	// Remove cell connection's connections
	for (const auto& currentConnection : pCell->pConnectedCells)
	{
		auto& connectionConnections{ currentConnection->pConnectedCells };
		connectionConnections.erase(std::remove(connectionConnections.begin(), connectionConnections.end(), pCell), connectionConnections.end());
	}

	// Clear connections
	pCell->pConnectedCells.clear();
}

void GridComponent::SetLevelFile(const std::string& levelFile)
{
	// Get file and open
	// -----------------
	auto jsonFile{ dae::ResourceManager::GetInstance().LoadFile(levelFile) };
	assert(jsonFile->is_open() && "Error: failed to open levelFile");

	// Parse File
	// ----------

	// Wraps around ifstream, used for the Parsing
	rapidjson::IStreamWrapper streamWrapper{ *jsonFile };

	// Parse File
	rapidjson::Document jsonDoc{};
	jsonDoc.ParseStream(streamWrapper);

	// Access data
	// -----------
	m_NrRows = jsonDoc["height"].GetInt();
	m_NrCols = jsonDoc["width"].GetInt();

	m_CellWidth = jsonDoc["tilewidth"].GetInt();
	m_CellHeight = jsonDoc["tileheight"].GetInt();

	auto textureIdArray{ jsonDoc["layers"].GetArray()[0]["data"].GetArray() };
	auto rockArray{ jsonDoc["layers"].GetArray()[1]["data"].GetArray()};
	auto enemiesArray{ jsonDoc["layers"].GetArray()[2]["data"].GetArray() };

	const unsigned int enemyID_Offset{ 26 };

	// Init grid
	// ---------
	InitGridCells();

	glm::vec3 cellPosition{};
	int arrayID{};
	std::pair<glm::vec3, unsigned int> enemySpawnInfo{};

	for (size_t idx{}; idx < m_pCells.size(); ++idx)
	{
		// Data
		cellPosition.x = m_pCells[idx]->centerPosition.x;
		cellPosition.y = m_pCells[idx]->centerPosition.y;

		// Texture
		m_pCells[idx]->textureID = textureIdArray[static_cast<rapidjson::SizeType>(idx)].GetInt();
		
		// Rocks
		if (rockArray[static_cast<rapidjson::SizeType>(idx)].GetInt() != 0)
		{
			m_pCells[idx]->containsRock = true;
			CreateRock(cellPosition);
		}

		// Enemies
		arrayID = enemiesArray[static_cast<rapidjson::SizeType>(idx)].GetInt();
		if (arrayID != 0)
		{
			enemySpawnInfo = std::make_pair(cellPosition, arrayID - enemyID_Offset);
			m_EnemySpawnData.emplace_back(enemySpawnInfo);
		}
	}

	// Connections
	// -----------
	InitCellConnections();

	// Close File
	// ----------
	jsonFile->close();
}
void GridComponent::SetRockTexture(const std::string& rockTexture)
{
	m_pRockTexture = dae::ResourceManager::GetInstance().LoadTexture(rockTexture);
}

// Member Functions
// ****************

void GridComponent::InitGridCells()
{
	// Create cells
	// ------------

	// Re-Init grid
	m_pCells.clear();
	m_pCells.resize(m_NrRows * m_NrCols);

	// Prepare loop
	Cell gridCell{};
	for (int rowIdx{}; rowIdx < m_NrRows; ++rowIdx)
	{
		for (int colIdx{}; colIdx < m_NrCols; ++colIdx)
		{
			const int gridIdx{ rowIdx * m_NrCols + colIdx };

			// Give cell data
			gridCell.size = glm::vec2{ m_CellWidth, m_CellHeight };
			gridCell.worldPosition = glm::vec2{ colIdx * m_CellWidth, rowIdx * m_CellHeight };
			gridCell.centerPosition = gridCell.worldPosition + gridCell.size / 2.f;
			gridCell.rowCol = glm::vec2{ rowIdx, colIdx };

			if (rowIdx == 0)		gridCell.depthLevel = 0;
			else if (rowIdx < 4)	gridCell.depthLevel = 1;
			else if (rowIdx < 8)	gridCell.depthLevel = 2;
			else					gridCell.depthLevel = 3;

			// Make cell
			m_pCells[gridIdx] = std::make_unique<Cell>(gridCell);
		}
	}
}
void GridComponent::CreateRock(const glm::vec3& rockPosition)
{
	// Create gameObject
	// -----------------
	std::shared_ptr<dae::GameObject> pRock{ std::make_shared<dae::GameObject>() };

	// Add components
	// --------------

	// Textures
	dae::RenderTextureComponent* pObjectTexture{ nullptr };
	if (m_pRockTexture)
	{
		pObjectTexture = pRock->AddComponent<dae::RenderTextureComponent>();
		pObjectTexture->SetTexture(m_pRockTexture);
		pObjectTexture->CenterTexture(true);
	}

	// Rock
	RockComponent* pRockComponent{ pRock->AddComponent<RockComponent>() };
	pRockComponent->SetGrid(this);
	pRockComponent->SetRenderTextureComponent(pObjectTexture);

	// Add as child
	// ------------
	pRock->SetWorldPosition(rockPosition);
	pRock->SetParent(GetGameObject(), true);
}

void GridComponent::InitCellConnections()
{
	// Prepare loop
	Cell* pCurrentCell{ nullptr };
	int gridIdx{};

	for (int rowIdx{}; rowIdx < m_NrRows; ++rowIdx)
	{
		for (int colIdx{}; colIdx < m_NrCols; ++colIdx)
		{
			// Get currentCell
			pCurrentCell = m_pCells[rowIdx * m_NrCols + colIdx].get();
			bool isValidCell = pCurrentCell->textureID == 0 && pCurrentCell->containsRock == false;
			if (isValidCell == false) continue;

			// Left cell
			gridIdx = rowIdx * m_NrCols + (colIdx - 1);
			CheckNeighborCell(pCurrentCell, gridIdx, true);

			// Right cell
			gridIdx = rowIdx * m_NrCols + (colIdx + 1);
			CheckNeighborCell(pCurrentCell, gridIdx, true);

			// Below cell
			gridIdx = (rowIdx + 1) * m_NrCols + colIdx;
			CheckNeighborCell(pCurrentCell, gridIdx, false);

			// Above cell
			gridIdx = (rowIdx - 1) * m_NrCols + colIdx;
			CheckNeighborCell(pCurrentCell, gridIdx, false);
		}
	}
}
void GridComponent::CheckNeighborCell(Cell* pCurrentCell, int cellIdx, bool checkIfSameRow)
{
	// Get neighbourCell
	// -----------------
	Cell* pNeighbourCell{ GetCell(cellIdx) };

	// Check conditions
	// ----------------

	// Check if is not null, ground or rock
	const bool isValidCell{ pNeighbourCell != nullptr && pNeighbourCell->textureID == 0 && pNeighbourCell->containsRock == false };
	if (isValidCell == false) return;
	
	// Check if cell is not on the otherside of the grid
	if (checkIfSameRow && pNeighbourCell->rowCol.x != pCurrentCell->rowCol.x) return;

	// Check if cell is not already in connectedCell
	auto neighborIt{ std::find(pCurrentCell->pConnectedCells.begin(), pCurrentCell->pConnectedCells.end(), pNeighbourCell) };
	if (neighborIt != pCurrentCell->pConnectedCells.end()) return;

	// Check if neighbour doesn't contain currentCell
	auto currentIt{ std::find(pNeighbourCell->pConnectedCells.begin(), pNeighbourCell->pConnectedCells.end(), pCurrentCell) };
	if (currentIt != pNeighbourCell->pConnectedCells.end()) return;

	// Add connections
	// ---------------
	pCurrentCell->pConnectedCells.emplace_back(pNeighbourCell);
	pNeighbourCell->pConnectedCells.emplace_back(pCurrentCell);
}

void GridComponent::RenderGrid() const
{
	if (m_pRenderer == nullptr) return;

	// Draw corresponding texture of each cell
	glm::vec2 pos{};
	utils::Rect srcRect{};
	srcRect.width = static_cast<float>(m_CellWidth);
	srcRect.height = static_cast<float>(m_CellHeight);

	for (const auto& currentCell : m_pCells)
	{
		// If textureID is valid
		if (currentCell->textureID != 0)
		{
			pos.x = currentCell->worldPosition.x;
			pos.y = currentCell->worldPosition.y;
			srcRect.x = (currentCell->textureID - 1) * srcRect.width;
			m_pRenderer->RenderManually(pos, srcRect);
		}
	}
}
void GridComponent::RenderDebugGrid() const
{
	// Prepare variables
	SDL_Rect rect{};
	rect.w = m_CellWidth;
	rect.h = m_CellHeight;

	int currentX{}, currentY{};
	int connectionX{}, connectionY{};

	// Draw Grid
	auto pRenderer{ dae::Renderer::GetInstance().GetSDLRenderer() };	
	for (const auto& currentCell : m_pCells)
	{
		// Individual cells
		SDL_SetRenderDrawColor(pRenderer, static_cast<Uint8>(255), static_cast<Uint8>(255), static_cast<Uint8>(255), static_cast<Uint8>(255));

		rect.x = static_cast<int>(static_cast<float>(currentCell->worldPosition.x));
		rect.y = static_cast<int>(static_cast<float>(currentCell->worldPosition.y));

		SDL_RenderDrawRect(pRenderer, &rect);

		// Connections
		SDL_SetRenderDrawColor(pRenderer, static_cast<Uint8>(255 / 2.f), static_cast<Uint8>(255 / 2.f), static_cast<Uint8>(255 / 2.f), static_cast<Uint8>(255));

		currentX = static_cast<int>(static_cast<float>(currentCell->centerPosition.x));
		currentY = static_cast<int>(static_cast<float>(currentCell->centerPosition.y));

		for (const auto& connectedCell : currentCell->pConnectedCells)
		{
			connectionX = static_cast<int>(static_cast<float>(connectedCell->centerPosition.x));
			connectionY = static_cast<int>(static_cast<float>(connectedCell->centerPosition.y));

			SDL_RenderDrawLine(pRenderer, currentX, currentY, connectionX, connectionY);
		}
	}
}