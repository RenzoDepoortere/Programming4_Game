#include "EnemyManager.h"

EnemyManager::EnemyManager(dae::GameObject* pParentObject)
	: Component{ pParentObject }
{
}

void EnemyManager::SpawnEnemiesAtPositions(const std::vector<std::pair<glm::vec3, unsigned int>>& /*spawnData*/)
{

}