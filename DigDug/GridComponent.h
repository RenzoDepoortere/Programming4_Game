#pragma once
#include "Component.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "Texture2D.h"

#include <vector>
#include <string>
#include <memory>

namespace dae
{
	class GameObject;
	class RenderTextureComponent;
}

namespace grid
{
	struct Cell
	{
		glm::vec2 worldPosition{ 0, 0 };
		glm::vec2 centerPosition{ 0,0 };
		glm::vec2 size{ 0, 0 };

		glm::vec2 rowCol{ 0,0 };

		bool containsRock{ false };
		int depthLevel{ 0 };

		unsigned int textureID{ 0 };
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
		Cell* GetCell(int index) const;
		Cell* GetCell(const glm::vec3& worldPos) const;
		Cell* GetCell(float x, float y, float z) const;

		void Render() const override;

		void SetLevelFile(const std::string& levelFile);
		void SetRockTexture(const std::string& rockTexture);

		int GetNrRows() const { return m_NrRows; }
		int GetNrCols() const { return m_NrCols; }
		int GetCellWidth() const { return m_CellWidth; }
		int GetCellHeight() const { return m_CellHeight; }

	private:
		// Member variables
		// ---------------

		// Array
		std::vector<std::unique_ptr<Cell>> m_Cells{};

		// Cell
		int m_NrRows{};
		int m_NrCols{};

		int m_CellWidth{};
		int m_CellHeight{};

		// Renderer
		dae::RenderTextureComponent* m_pRenderer{ nullptr };

		// Rocks
		std::shared_ptr<dae::Texture2D> m_pRockTexture{ nullptr };

		// Member functions
		// ----------------
		void InitGridCells();
		void CreateRock(const glm::vec3& rockPosition);

		void RenderGrid() const;
		void RenderDebugGrid() const;
	};
}