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
	if (0 <= index && index < m_Cells.size())
	{
		return m_Cells[index].get();
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

	// Init grid
	// ---------
	InitGridCells();

	glm::vec3 rockPosition{};
	for (size_t idx{}; idx < m_Cells.size(); ++idx)
	{
		// Texture
		m_Cells[idx]->textureID = textureIdArray[static_cast<rapidjson::SizeType>(idx)].GetInt();
		
		// Rocks
		if (rockArray[static_cast<rapidjson::SizeType>(idx)].GetInt() != 0)
		{
			m_Cells[idx]->containsRock = true;

			rockPosition.x = m_Cells[idx]->centerPosition.x;
			rockPosition.y = m_Cells[idx]->centerPosition.y;
			CreateRock(rockPosition);
		}
	}

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
	m_Cells.clear();
	m_Cells.resize(m_NrRows * m_NrCols);

	Cell gridCell{};
	for (int rowIdx{}; rowIdx < m_NrRows; ++rowIdx)
	{
		for (int colIdx{}; colIdx < m_NrCols; ++colIdx)
		{
			const int gridIdx{ rowIdx * m_NrCols + colIdx };

			gridCell.size = glm::vec2{ m_CellWidth, m_CellHeight };
			gridCell.worldPosition = glm::vec2{ colIdx * m_CellWidth, rowIdx * m_CellHeight };
			gridCell.centerPosition = gridCell.worldPosition + gridCell.size / 2.f;
			gridCell.rowCol = glm::vec2{ rowIdx, colIdx };

			if (rowIdx == 0)		gridCell.depthLevel = 0;
			else if (rowIdx < 4)	gridCell.depthLevel = 1;
			else if (rowIdx < 8)	gridCell.depthLevel = 2;
			else					gridCell.depthLevel = 3;

			m_Cells[gridIdx] = std::make_unique<Cell>(gridCell);
		}
	}
}
void GridComponent::GridComponent::CreateRock(const glm::vec3& rockPosition)
{
	// Create gameObject
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
	pRock->SetWorldPosition(rockPosition);
	pRock->SetParent(GetGameObject(), true);
}

void GridComponent::RenderGrid() const
{
	if (m_pRenderer == nullptr) return;

	// Draw corresponding texture of each cell
	utils::Rect destRect{};
	utils::Rect srcRect{};
	srcRect.width = static_cast<float>(m_CellWidth);
	srcRect.height = static_cast<float>(m_CellHeight);

	for (const auto& currentCell : m_Cells)
	{
		// If textureID is valid
		if (currentCell->textureID != 0)
		{
			destRect.x = currentCell->worldPosition.x;
			destRect.y = currentCell->worldPosition.y;
			srcRect.x = (currentCell->textureID - 1) * srcRect.width;
			m_pRenderer->RenderManually(destRect, srcRect, 0.f);
		}
	}
}
void GridComponent::RenderDebugGrid() const
{
	// Init rect
	SDL_Rect rect{};
	rect.w = m_CellWidth;
	rect.h = m_CellHeight;

	// Draw rects
	auto pRenderer{ dae::Renderer::GetInstance().GetSDLRenderer() };
	SDL_SetRenderDrawColor(pRenderer, static_cast<Uint8>(255), static_cast<Uint8>(255), static_cast<Uint8>(255), static_cast<Uint8>(255));
	for (const auto& currentCell : m_Cells)
	{
		rect.x = static_cast<int>(static_cast<float>(currentCell->worldPosition.x));
		rect.y = static_cast<int>(static_cast<float>(currentCell->worldPosition.y));

		SDL_RenderDrawRect(pRenderer, &rect);
	}
}