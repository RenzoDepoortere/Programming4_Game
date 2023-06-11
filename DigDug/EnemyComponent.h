#pragma once
#include "Component.h"
#include "MoveCommand.h"
#include "EnemyState.h"
#include "EnemyManager.h"
#include "Observer.h"

#include <array>
#include <functional>

namespace enemy
{
	struct BehaviorData
	{
		float movementSpeed{};

		int detectionRange{};		// How many cells the enemy can max observe in each direction
		float detectionInterval{};	// How long it takes before the enemy checks whether it should still chase or give up

		float ghostTime{};			// How long it takes before the enemy turns into a ghost
		float maxGhostTime{};		// How long the enemy can be in ghost mode

		float attackTime{};			// How long it will take before the enemy randomly attack
		float attackCharge{};		// How long it will take the enemy before actually attacking
		float attackStun{};			// How long the enemy will be stunend after it attacks

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

class EnemyComponent final : public Component, public dae::Observer<void*>
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
	void Reset();
	void StoreParent(dae::GameObject* pParent) { m_pParent = pParent; }

	virtual void Update(float deltaTime) override;
	virtual void Render() const override;
	
	void SetCaught(bool isCaught);
	bool GetCaught() const { return m_IsCaught; }
	void SetBlow(bool blow) { m_Blown = blow; }
	bool GetBlown() const { return m_Blown; }

	void SetSquashed();
	void SetFlee();
	bool GetIsFleeing() const { return m_IsFleeing; }

	enemy::EnemyStates GetCurrentStateID() const { return m_CurrentStateID; }

	void SetBehaviorData(const enemy::BehaviorData& behaviorData) { m_EnemyBehavior = behaviorData; }
	enemy::BehaviorData GetBehaviorData() const { return m_EnemyBehavior; }

	dae::AnimationComponent* GetAnimationComponent();
	bool GetIsLookingLeft() const { return m_IsLookingLeft; }

	void SetIsControlled(bool isControlled) { m_IsControlled = isControlled; }
	bool GetIsControlled() const { return m_IsControlled; }

	// Observer
	virtual void HandleEvent(unsigned int eventID, void* extraInfo) override;
	virtual void OnSubjectDestroy() override;

private:
	// Member variables
	// ----------------
	dae::AnimationComponent* m_pAnimationComponent{ nullptr };
	dae::GameObject* m_pParent{ nullptr };
	bool m_StateInitialized{};

	bool m_IsControlled{ false };

	bool m_IsCaught{ false };
	bool m_Blown{ false };
	bool m_IsFleeing{ false };
	bool m_IsDead{ false };

	bool m_IsLookingLeft{ false };
	float m_PreviousX{};

	enemy::BehaviorData m_EnemyBehavior{};
	enemy::EnemyState* m_pCurrentState{};
	enemy::EnemyStates m_CurrentStateID{};
	std::array<std::unique_ptr<enemy::EnemyState>, enemy::NR_STATES> m_pEnemyStates{};

	// Member functions
	// ----------------	
	void InitStates();
	void OnInactive();

	void UpdateStates(float deltaTime);
	void CheckPlayer();
	void HandleRotating();
};