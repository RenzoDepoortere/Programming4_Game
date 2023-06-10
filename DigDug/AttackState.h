#pragma once
#include "EnemyState.h"
#include "Texture2D.h"

#include <memory>

namespace dae
{
	class AnimationComponent;
}

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
		// Member variables
		// ----------------
		std::shared_ptr<dae::Texture2D> m_pFygarFireTexture{ nullptr };
		dae::AnimationComponent* m_pAnimationComponent{ nullptr };

		float m_CurrentTime{};
	};
}