#include "GridComponent.h"
#include "Renderer.h"
#include "GameObject.h"
#include "RenderTextureComponent.h"

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

Cell GridComponent::GetCell(int index) const
{
	if (0 <= index && index < m_Cells.size())
	{
		return m_Cells[index];
	}
	else
	{
		//std::cout << "Error: Tried to get cell with invalid index" << std::endl;
		return {};
	}
}
Cell GridComponent::GetCell(const glm::vec3& worldPos) const
{
	return GetCell(worldPos.x, worldPos.y, worldPos.z);
}
Cell GridComponent::GetCell(float x, float y, float) const
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
	RenderDebugGrid();
}

void GridComponent::SetLevelFile(const std::string& /*levelFile*/)
{
<<<<<<< Updated upstream

=======
	// Get file and open
	// -----------------
	auto jsonFile{ dae::ResourceManager::GetInstance().LoadFile(levelFile) };
	assert(jsonFile->is_open() && "Error: failed to open levelFile");

	// Parse File
	// ----------

	// Wraps around ifstream, used for the Parsing
	rapidjson::IStreamWrapper streamWrapper{ *jsonFile };
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
		m_Cells[idx].textureID = textureIdArray[static_cast<rapidjson::SizeType>(idx)].GetInt();
		
		// Rocks
		if (rockArray[static_cast<rapidjson::SizeType>(idx)].GetInt() != 0)
		{
			m_Cells[idx].containsRock = true;

			rockPosition.x = m_Cells[idx].centerPosition.x;
			rockPosition.y = m_Cells[idx].centerPosition.y;
			CreateRock(rockPosition);
		}
	}

	// Close File
	// ----------
	jsonFile->close();
>>>>>>> Stashed changes
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
			gridCell.size = glm::vec2{ m_CellWidth, m_CellHeight };
			gridCell.worldPosition = glm::vec2{ colIdx * m_CellWidth, rowIdx * m_CellHeight };
			gridCell.centerPosition = gridCell.worldPosition + gridCell.size / 2.f;
			gridCell.rowCol = glm::vec2{ rowIdx, colIdx };

			if (rowIdx == 0)		gridCell.depthLevel = 0;
			else if (rowIdx < 4)	gridCell.depthLevel = 1;
			else if (rowIdx < 8)	gridCell.depthLevel = 2;
			else					gridCell.depthLevel = 3;

			const int gridIdx{ rowIdx * m_NrCols + colIdx };
			m_Cells[gridIdx] = gridCell;
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
	if (m_pRockTexture)
	{
		dae::RenderTextureComponent* pObjectTexture{ pRock->AddComponent<dae::RenderTextureComponent>() };
		pObjectTexture->SetTexture(m_pRockTexture);
		pObjectTexture->CenterTexture(true);
	}

	// Rock

	// Add as child
	pRock->SetWorldPosition(rockPosition);
	pRock->SetParent(GetGameObject(), true);
}

void GridComponent::RenderGrid() const
{

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
		rect.x = static_cast<int>(static_cast<float>(currentCell.worldPosition.x));
		rect.y = static_cast<int>(static_cast<float>(currentCell.worldPosition.y));

		SDL_RenderDrawRect(pRenderer, &rect);
	}
}