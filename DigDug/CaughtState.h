#pragma once
#include "EnemyState.h"
#include "Texture2D.h"

#include <array>
#include <memory>

namespace dae
{
	class AnimationComponent;
}

namespace enemy
{
	class CaughtState final : public EnemyState
	{
	public:
		// Rule of Five
		CaughtState();
		virtual ~CaughtState() override = default;

		CaughtState(const CaughtState& other) = delete;
		CaughtState(CaughtState&& other) = delete;
		CaughtState& operator=(const CaughtState& other) = delete;
		CaughtState& operator=(CaughtState&& other) = delete;

		// Functionality
		virtual void OnEnter(EnemyComponent* pEnemy) override;
		virtual void OnLeave(EnemyComponent* pEnemy) override;

		virtual EnemyStates Update(EnemyComponent* pEnemy, float deltaTime) override;

	private:
		// Member variables
		// ----------------
		std::array<std::pair<std::shared_ptr<dae::Texture2D>, float>, 4> m_pPookaCaughtTextures{};
		std::array<std::pair<std::shared_ptr<dae::Texture2D>, float>, 4> m_pFygarCaughtTextures{};
		std::array<std::pair<std::shared_ptr<dae::Texture2D>, float>, 4> m_pCurrentCaughtTextures{};
		dae::AnimationComponent* m_pAnimationComponent{ nullptr };

		int m_CurrentState{};
		float m_CurrentTime{};


		// Member functions
		// ----------------
		EnemyStates HandleCaughtGrading(EnemyComponent* pEnemy, float deltaTime);
	};
}