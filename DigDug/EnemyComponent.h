#pragma once
#include "Component.h"

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


	void SetGrid(grid::GridComponent* pGrid) { m_pGrid = pGrid; }
	void SetBehaviorData(const Enemy::BehaviorData& behaviorData) { m_EnemyBehavior = behaviorData; }
	Enemy::BehaviorData GetBehaviorData() const { return m_EnemyBehavior; }

private:
	// Member variables
	// ----------------
	grid::GridComponent* m_pGrid{ nullptr };

	Enemy::BehaviorData m_EnemyBehavior{};
};