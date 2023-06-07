#pragma once
#include "EnemyState.h"

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

		// Member functions
		// ----------------

	};
}