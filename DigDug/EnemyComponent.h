#pragma once
#include "Component.h"

#include "MoveCommand.h"

namespace Enemy
{
	struct BehaviorData
	{
		float movementSpeed{};

		float detectionRange{};
		float attackRange{};

		float attackCooldown{};
		//std::function attackFunction{};
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

	void CreateMovementCommand();

	void SetGrid(grid::GridComponent* pGrid) { m_pGrid = pGrid; }
	void SetBehaviorData(const Enemy::BehaviorData& behaviorData) { m_EnemyBehavior = behaviorData; }
	Enemy::BehaviorData GetBehaviorData() const { return m_EnemyBehavior; }

private:
	// Member variables
	// ----------------
	grid::GridComponent* m_pGrid{ nullptr };

	Enemy::BehaviorData m_EnemyBehavior{};

	std::unique_ptr<dae::MoveCommand> m_pMoveCommand{ nullptr };
	grid::Cell* m_pPreviousCell{ nullptr };
	grid::Cell* m_pNextCell{ nullptr };

	// Member functions
	// ----------------
	void TempPathing(float deltaTime);
	void FindNextCell(grid::Cell* pCurrentCell);
};