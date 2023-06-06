#pragma once
#include "Component.h"
#include "glm/vec3.hpp"

#include <vector>

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

	// Public functions
	void SpawnEnemiesAtPositions(const std::vector<std::pair<glm::vec3, unsigned int>>& spawnData);

private:
	// Member variables
	// ----------------
	
	//std::vector<
};