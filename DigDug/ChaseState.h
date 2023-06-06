#pragma once
#include "EnemyState.h"
#include "MoveCommand.h"

#include <memory>
#include <array>

class EnemyComponent;

namespace grid
{
	class GridComponent;
	struct Cell;
}

namespace Enemy
{
	class ChaseState final : public EnemyState
	{
	public:
		// Rule of Five
		ChaseState() = default;
		virtual ~ChaseState() override = default;

		ChaseState(const ChaseState& other) = delete;
		ChaseState(ChaseState&& other) = delete;
		ChaseState& operator=(const ChaseState& other) = delete;
		ChaseState& operator=(ChaseState&& other) = delete;

		// Functionality
		virtual EnemyStates Update(EnemyComponent* pEnemy, float deltaTime) override;

		virtual void OnEnter(EnemyComponent* pEnemy) override;
		virtual void OnLeave(EnemyComponent* pEnemy) override;

	private:
		// Member variables
		// ----------------

	};
}