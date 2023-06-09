#pragma once
#include "EnemyState.h"
#include "MoveCommand.h"
#include "Texture2D.h"

#include <memory>
#include <array>

class EnemyComponent;

namespace grid
{
	struct Cell;
}

namespace enemy
{
	class RoamingState final : public EnemyState
	{
	public:
		// Rule of Five
		RoamingState();
		virtual ~RoamingState() override = default;

		RoamingState(const RoamingState& other) = delete;
		RoamingState(RoamingState&& other) = delete;
		RoamingState& operator=(const RoamingState& other) = delete;
		RoamingState& operator=(RoamingState&& other) = delete;

		// Functionality
		virtual void OnEnter(EnemyComponent* pEnemy) override;
		virtual void OnLeave(EnemyComponent* pEnemy) override;

		virtual EnemyStates Update(EnemyComponent* pEnemy, float deltaTime) override;

	private:	
		// Member variables
		// ----------------
		std::shared_ptr<dae::Texture2D> m_pPookaWalkingAnimation{ nullptr };
		std::shared_ptr<dae::Texture2D> m_pFygarWalkingAnimation{ nullptr };

		std::array<std::unique_ptr<dae::MoveCommand>, 4> m_pMoveCommands{};
		dae::MoveCommand* m_pCurrentCommand{ nullptr };

		bool m_CommandInitialized{ false };

		grid::Cell* m_pPreviousCell{ nullptr };
		grid::Cell* m_pNextCell{ nullptr };

		// Member functions
		// ----------------
		void InitMovementCommands(EnemyComponent* pEnemy);

		void HandlePathing(EnemyComponent* pEnemy, float deltaTime);
		void FindNextCell(grid::Cell* pCurrentCell);

		EnemyStates LookForPlayer(EnemyComponent* pEnemy, float deltaTime);
		bool IsDirtBetween(int cellsBetween, bool checkRows, grid::GridComponent* pGrid, grid::Cell* pCurrentCell, grid::Cell* pCharacterCell);
	};
}