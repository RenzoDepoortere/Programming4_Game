#pragma once
#include "EnemyState.h"
#include "MoveCommand.h"

#include <memory>

class EnemyComponent;

namespace grid
{
	class GridComponent;
	struct Cell;
}

namespace Enemy
{
	class RoamingState final : public EnemyState
	{
	public:
		// Rule of Five
		RoamingState() = default;
		virtual ~RoamingState() override = default;

		RoamingState(const RoamingState& other) = delete;
		RoamingState(RoamingState&& other) = delete;
		RoamingState& operator=(const RoamingState& other) = delete;
		RoamingState& operator=(RoamingState&& other) = delete;

		// Functionality
		virtual void Update(EnemyComponent* pEnemy, float deltaTime) override;

		virtual void OnEnter(EnemyComponent* pEnemy) override;
		virtual void OnLeave(EnemyComponent* pEnemy) override;

	private:
		// Member variables
		// ----------------
		std::unique_ptr<dae::MoveCommand> m_pMoveCommand{ nullptr };
		grid::Cell* m_pPreviousCell{ nullptr };
		grid::Cell* m_pNextCell{ nullptr };

		// Member functions
		// ----------------
		void HandlePathing(EnemyComponent* pEnemy, float deltaTime);
		void FindNextCell(grid::Cell* pCurrentCell);
	};
}