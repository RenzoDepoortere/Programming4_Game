#pragma once
#include "Component.h"
#include "MoveCommand.h"
#include "EnemyState.h"
#include "EnemyManager.h"

#include <array>

namespace Enemy
{
	struct BehaviorData
	{
		float movementSpeed{};

		// How many cells the enemy can max observe in each direction
		int detectionRange{};
		float attackRange{};

		float attackCooldown{};
		//std::function attackFunction{};

		EnemyTypes enemyType{};
	};
}

namespace grid
{
	class GridComponent;
	struct Cell;
}

class EnemyComponent final : public Component
{
public:
	// Rule of Five
	explicit EnemyComponent(dae::GameObject* pParentObject);
	virtual ~EnemyComponent() override = default;

	EnemyComponent(const EnemyComponent& other) = delete;
	EnemyComponent(EnemyComponent&& other) = delete;
	EnemyComponent& operator=(const EnemyComponent& other) = delete;
	EnemyComponent& operator=(EnemyComponent&& other) = delete;

	// Functionality
	virtual void Update(float deltaTime) override;

	void SetGrid(grid::GridComponent* pGrid) { m_pGrid = pGrid; }
	grid::GridComponent* GetGrid() const { return m_pGrid; }

	void SetCharacters(const std::vector<CharacterComponent*>& pCharacters) { m_pCharacters = pCharacters; }
	const std::vector<CharacterComponent*>& GetCharacters() const { return m_pCharacters; }

	void SetBehaviorData(const Enemy::BehaviorData& behaviorData) { m_EnemyBehavior = behaviorData; }
	Enemy::BehaviorData GetBehaviorData() const { return m_EnemyBehavior; }

	void SetControl(unsigned long controllerID = -1);
	bool GetIsControlled() const { return m_IsControlled; }
	unsigned long GetControllerID() const { return m_ControllerID; }

private:
	// Member variables
	// ----------------
	grid::GridComponent* m_pGrid{ nullptr };
	std::vector<CharacterComponent*> m_pCharacters{};
	bool m_InitializedStates{ false };

	bool m_IsControlled{ false };
	unsigned long m_ControllerID{};

	Enemy::BehaviorData m_EnemyBehavior{};
	Enemy::EnemyState* m_pCurrentState{};
	std::array<std::unique_ptr<Enemy::EnemyState>, Enemy::NR_STATES> m_pEnemyStates{};

	// Member functions
	// ----------------	
	void InitStates();
};