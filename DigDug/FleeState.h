#pragma once
#include "EnemyState.h"
#include "MoveCommand.h"

#include <memory>

namespace enemy
{
	class FleeState final : public EnemyState
	{
	public:
		// Rule of Five
		FleeState() = default;
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
		std::unique_ptr<dae::MoveCommand> m_pMoveCommand{ nullptr };

		// Member functions
		// ----------------
		void HandleMovement(EnemyComponent* pEnemy, float deltaTime);
	};
}