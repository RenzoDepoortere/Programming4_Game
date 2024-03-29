#pragma once
#include "EnemyState.h"
#include "Texture2D.h"

#include <memory>

namespace dae
{
	class AnimationComponent;
}

class FireComponent;

namespace enemy
{
	class AttackState final : public EnemyState
	{
	public:
		// Rule of Five
		AttackState();
		virtual ~AttackState() override = default;

		AttackState(const AttackState& other) = delete;
		AttackState(AttackState&& other) = delete;
		AttackState& operator=(const AttackState& other) = delete;
		AttackState& operator=(AttackState&& other) = delete;

		// Functionality
		virtual void OnEnter(EnemyComponent* pEnemy) override;
		virtual void OnLeave(EnemyComponent* pEnemy) override;

		virtual EnemyStates Update(EnemyComponent* pEnemy, float deltaTime) override;

	private:
		// Enums
		// -----
		enum AttackPhase
		{
			Prepare, Attack, Stun
		};

		// Member variables
		// ----------------
		std::shared_ptr<dae::Texture2D> m_pPrepareTexture{ nullptr };
		dae::AnimationComponent* m_pAnimationComponent{ nullptr };

		AttackPhase m_CurrentPhase{};
		float m_CurrentTime{};

		bool m_FireInitialized{ false };
		FireComponent* m_pFireComponent{ nullptr };

		// Member functions
		// ----------------
		void InitFire(EnemyComponent* pEnemy);

		void PrepareAttack(EnemyComponent* pEnemy, float deltaTime);
		void HandleAttack(float deltaTime);
		EnemyStates HoldStun(EnemyComponent* pEnemy, float deltaTime);
	};
}