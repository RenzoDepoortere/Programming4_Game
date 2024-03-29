#pragma once
#include "Component.h"
#include "glm/vec3.hpp"
#include "Texture2D.h"
#include "Observer.h"

#include <vector>

namespace grid
{
	class GridComponent;
	struct Cell;
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

class EnemyManager final : public Component, public dae::Observer<grid::Cell*, void*, void*>
{
public:
	// Rule of Five
	explicit EnemyManager(dae::GameObject* pParentObject);
	virtual ~EnemyManager() override;

	EnemyManager(const EnemyManager& other) = delete;
	EnemyManager(EnemyManager&& other) = delete;
	EnemyManager& operator=(const EnemyManager& other) = delete;
	EnemyManager& operator=(EnemyManager&& other) = delete;

	// Functionality
	void Reset();

	void SpawnEnemies();
	void ControlEnemy(enemy::EnemyTypes enemyType);

	const std::vector<EnemyComponent*>& GetEnemies() const { return m_pEnemies; }

	// Observer
	virtual void HandleEvent(unsigned int eventID, grid::Cell* pCell, void* behaviorData, void* wasSquashed) override;
	virtual void OnSubjectDestroy() override;

private:
	// Member variables
	// ----------------
	std::vector<EnemyComponent*> m_pEnemies{};

	// Member functions
	// ----------------
	void SpawnPooka(const glm::vec3& position);
	void SpawnFygar(const glm::vec3& position);
};