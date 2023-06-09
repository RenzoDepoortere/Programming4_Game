#pragma once
#include "Component.h"
#include "glm/vec3.hpp"
#include "Texture2D.h"

#include <vector>

namespace grid
{
	class GridComponent;
}

namespace enemy
{
	enum EnemyTypes
	{
		Pooka, Fygar
	};
}

class CharacterComponent;
class EnemyComponent;

class EnemyManager final : public Component
{
public:
	// Rule of Five
	explicit EnemyManager(dae::GameObject* pParentObject);
	virtual ~EnemyManager() override = default;

	EnemyManager(const EnemyManager& other) = delete;
	EnemyManager(EnemyManager&& other) = delete;
	EnemyManager& operator=(const EnemyManager& other) = delete;
	EnemyManager& operator=(EnemyManager&& other) = delete;

	// Functionality
	void SpawnEnemies();
	void ControlEnemy(unsigned long controllerID, enemy::EnemyTypes enemyType);

	void SetGrid(grid::GridComponent* pGrid) { m_pGrid = pGrid; }
	void SetCharacters(const std::vector<CharacterComponent*>& pCharacters);
	const std::vector<EnemyComponent*>& GetEnemies() const { return m_pEnemies; }

	bool CollidesEnemy(const glm::vec3 position, EnemyComponent*& pEnemy) const;

private:
	// Member variables
	// ----------------
	grid::GridComponent* m_pGrid{ nullptr };
	std::vector<CharacterComponent*> m_pCharacters{};

	std::vector<EnemyComponent*> m_pEnemies{};

	// Member functions
	// ----------------
	void SpawnPooka(const glm::vec3& position);
	void SpawnFygar(const glm::vec3& position);
};