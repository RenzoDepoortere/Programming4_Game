#include "GridComponent.h"
#include "Renderer.h"

#include <iostream>
#include <SDL.h>

using namespace grid;

GridComponent::GridComponent(dae::GameObject* pParentObject)
	: Component{ pParentObject }
{
	// Defaults
	m_NrRows = 12;
	m_NrCols = 12;

	m_CellWidth = 40;
	m_CellHeight = 40;

	// Init Grid
	Cell gridCell{};
	for (int rowIdx{}; rowIdx < m_NrRows; ++rowIdx)
	{
		for (int colIdx{}; colIdx < m_NrCols; ++colIdx)
		{
			gridCell.size = glm::vec2{ m_CellWidth, m_CellHeight };
			gridCell.worldPosition = glm::vec2{ colIdx * m_CellWidth, rowIdx * m_CellHeight };

			if (rowIdx == 0)		gridCell.depthLevel = 0;
			else if (rowIdx < 4)	gridCell.depthLevel = 1;
			else if (rowIdx < 8)	gridCell.depthLevel = 2;
			else					gridCell.depthLevel = 3;

			m_Cells.push_back(gridCell);
		}
	}
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
	const int gridWidth{ m_NrCols * m_CellWidth };
	const int gridHeight{ m_NrRows * m_CellHeight };

	if (gridWidth < worldPos.x || gridHeight < worldPos.y)
	{
		//std::cout << "Error: Tried to get cell with invalid worldPos" << std::endl;
		return {};
	}
	else
	{
		const int colIdx{ static_cast<int>(worldPos.x / m_CellWidth) };
		const int rowIdx{ static_cast<int>(worldPos.y / m_CellHeight) };

		const int cellIdx{ rowIdx * m_NrCols + colIdx };
		return GetCell(cellIdx);
	}
}

void GridComponent::Render() const
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