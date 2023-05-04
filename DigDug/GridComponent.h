#pragma once
#include "Component.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <vector>

namespace dae
{
	class GameObject;
}

namespace grid
{
	struct Cell
	{
		// Variables
		glm::vec2 worldPosition{ 0, 0 };
		glm::vec2 centerPosition{ 0,0 };
		glm::vec2 size{ 0, 0 };

		glm::vec2 rowCol{ 0,0 };

		bool containsRock{ false };
		int depthLevel{ 0 };

		// Functions
		bool operator== (const Cell& rhs)
		{
			if (worldPosition == rhs.worldPosition	 &&
				centerPosition == rhs.centerPosition &&
				size == rhs.size					 &&
				rowCol == rhs.rowCol				 &&
				containsRock == rhs.containsRock	 &&
				depthLevel == rhs.depthLevel)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	};

	class GridComponent final : public Component
	{
	public:
		// Rule of Five
		explicit GridComponent(dae::GameObject* pParentObject);
		virtual ~GridComponent() override = default;

		GridComponent(const GridComponent& other) = delete;
		GridComponent(GridComponent&& other) = delete;
		GridComponent& operator=(const GridComponent& other) = delete;
		GridComponent& operator=(GridComponent&& other) = delete;

		// Functionality
		Cell GetCell(int index) const;
		Cell GetCell(const glm::vec3& worldPos) const;
		Cell GetCell(float x, float y, float z) const;

		void Render() const override;

		// Todo: Change grid accordingly
		void SetNrRows(int nrRows) { m_NrRows = nrRows; }
		void SetNrCols(int nrCols) { m_NrCols = nrCols; }
		void SetCellWidth(int width) { m_CellWidth = width; }
		void SetCellHeight(int height) { m_CellHeight = height; }

		int GetNrRows() const { return m_NrRows; }
		int GetNrCols() const { return m_NrCols; }
		int GetCellWidth() const { return m_CellWidth; }
		int GetCellHeight() const { return m_CellHeight; }

	private:
		// Member variables
		// ---------------

		// Array
		std::vector<Cell> m_Cells{};

		// Cell
		int m_NrRows{};
		int m_NrCols{};

		int m_CellWidth{};
		int m_CellHeight{};
	};
}