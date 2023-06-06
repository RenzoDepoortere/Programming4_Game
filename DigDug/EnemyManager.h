#pragma once
#include "Component.h"
#include "glm/vec3.hpp"
#include "Texture2D.h"

#include <vector>

namespace grid
{
	class GridComponent;
}
class CharacterComponent;

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
	
	//This will spawn the enemies with the given data as well
	void SetGrid(grid::GridComponent* pGrid);
	void SetCharacters(const std::vector<CharacterComponent*>& pCharacters) { m_pCharacters = pCharacters; }

private:
	// Member variables
	// ----------------
	
	grid::GridComponent* m_pGrid{ nullptr };
	std::vector<CharacterComponent*> m_pCharacters{};

	std::shared_ptr<dae::Texture2D> m_pPookaTexture{ nullptr };
	std::shared_ptr<dae::Texture2D> m_pFygarTexture{ nullptr };

	// Member functions
	// ----------------
	void SpawnEnemies();

	void SpawnPooka(const glm::vec3& position);
	void SpawnFygar(const glm::vec3& position);
};