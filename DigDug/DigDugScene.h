#pragma once
#include <vector>

class CharacterComponent;
class EnemyComponent;

namespace grid
{
	class GridComponent;
}

namespace digdug
{
	class DigDugScene
	{
	public:
		// Rule of five
		DigDugScene() = default;
		virtual ~DigDugScene() = default;

		DigDugScene(const DigDugScene& other) = delete;
		DigDugScene(DigDugScene&& other) = delete;
		DigDugScene& operator=(const DigDugScene& other) = delete;
		DigDugScene& operator=(DigDugScene&& other) = delete;

		// Functionality
		virtual void SetActive(bool isActive) = 0;

		virtual grid::GridComponent* GetGrid() const = 0;
		virtual const std::vector<CharacterComponent*>& GetCharacters() const = 0;
		virtual const std::vector<EnemyComponent*>& GetEnemies() const = 0;
	};
}