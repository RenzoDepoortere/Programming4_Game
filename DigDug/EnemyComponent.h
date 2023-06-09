#pragma once
#include "Component.h"
#include "MoveCommand.h"
#include "EnemyState.h"
#include "EnemyManager.h"

#include <array>

namespace enemy
{
	struct BehaviorData
	{
		float movementSpeed{};

		int detectionRange{};		// How many cells the enemy can max observe in each direction
		float detectionInterval{};	// How long it takes before the enemy checks whether it should still chase or give up

		float ghostTime{};			// How long it takes before the enemy turns into a ghost
		float maxGhostTime{};		// How long the enemy can be in ghost mode

		//float attackRange{};
		//float attackCooldown{};
		//std::function attackFunction{};

		EnemyTypes enemyType{};
	};
}

namespace grid
{
	class GridComponent;
	struct Cell;
}

namespace dae
{
	class AnimationComponent;
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

	bool IsInsideEnemy(const glm::vec3 position) const;
	
	void SetCaught(bool isCaught);
	bool GetCaught() const { return m_IsCaught; }
	void SetBlow(bool blow) { m_Blown = blow; }
	bool GetBlown() const { return m_Blown; }

	void SetGrid(grid::GridComponent* pGrid) { m_pGrid = pGrid; }
	grid::GridComponent* GetGrid() const { return m_pGrid; }

	void SetCharacters(const std::vector<CharacterComponent*>& pCharacters) { m_pCharacters = pCharacters; }
	const std::vector<CharacterComponent*>& GetCharacters() const { return m_pCharacters; }

	void SetBehaviorData(const enemy::BehaviorData& behaviorData) { m_EnemyBehavior = behaviorData; }
	enemy::BehaviorData GetBehaviorData() const { return m_EnemyBehavior; }

	void SetAnimationComponent(dae::AnimationComponent* pAnimationComponent) { m_pAnimationComponent = pAnimationComponent; }
	dae::AnimationComponent* GetAnimationComponent() const { return m_pAnimationComponent; }

	void SetControl(unsigned long controllerID = -1);
	bool GetIsControlled() const { return m_IsControlled; }
	unsigned long GetControllerID() const { return m_ControllerID; }

private:
	// Member variables
	// ----------------
	grid::GridComponent* m_pGrid{ nullptr };
	dae::AnimationComponent* m_pAnimationComponent{ nullptr };
	std::vector<CharacterComponent*> m_pCharacters{};
	bool m_InitializedStates{ false };

	bool m_IsControlled{ false };
	unsigned long m_ControllerID{};

	bool m_IsCaught{ false };
	bool m_Blown{ false };

	enemy::BehaviorData m_EnemyBehavior{};
	enemy::EnemyState* m_pCurrentState{};
	std::array<std::unique_ptr<enemy::EnemyState>, enemy::NR_STATES> m_pEnemyStates{};

	// Member functions
	// ----------------	
	void InitStates();
};