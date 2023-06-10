#pragma once
#include "EnemyState.h"
#include "MoveCommand.h"
#include "Texture2D.h"

#include <memory>
#include <deque>

namespace grid
{
	struct Cell;
}

namespace enemy
{
	class FleeState final : public EnemyState
	{
	public:
		// Rule of Five
		FleeState();
		virtual ~FleeState() override = default;

		FleeState(const FleeState& other) = delete;
		FleeState(FleeState&& other) = delete;
		FleeState& operator=(const FleeState& other) = delete;
		FleeState& operator=(FleeState&& other) = delete;

		// Functionality
		virtual void OnEnter(EnemyComponent* pEnemy) override;
		virtual void OnLeave(EnemyComponent* pEnemy) override;

		virtual EnemyStates Update(EnemyComponent* pEnemy, float deltaTime) override;

	private:
		// Member variables
		// ----------------
		std::shared_ptr<dae::Texture2D> m_pPookaWalkingAnimation{ nullptr };
		std::shared_ptr<dae::Texture2D> m_pFygarWalkingAnimation{ nullptr };

		std::unique_ptr<dae::MoveCommand> m_pMoveCommand{ nullptr };
		std::deque<grid::Cell*> m_pPathToFollow{};

		float m_CurrentTime{};

		// Member functions
		// ----------------
		void FollowPath(EnemyComponent* pEnemy, float deltaTime);
		void HandleReachingFinish(EnemyComponent* pEnemy);
	};
}