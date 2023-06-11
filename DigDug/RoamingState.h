#pragma once
#include "EnemyState.h"
#include "MoveCommand.h"
#include "Texture2D.h"
#include "Observer.h"

#include <memory>
#include <array>

class EnemyComponent;

namespace grid
{
	struct Cell;
}

namespace enemy
{
	class RoamingState final : public EnemyState, public dae::Observer<float>
	{
	public:
		// Rule of Five
		RoamingState();
		virtual ~RoamingState() override;

		RoamingState(const RoamingState& other) = delete;
		RoamingState(RoamingState&& other) = delete;
		RoamingState& operator=(const RoamingState& other) = delete;
		RoamingState& operator=(RoamingState&& other) = delete;

		// Functionality
		virtual void OnEnter(EnemyComponent* pEnemy) override;
		virtual void OnLeave(EnemyComponent* pEnemy) override;

		virtual EnemyStates Update(EnemyComponent* pEnemy, float deltaTime) override;

		// Observer
		void HandleEvent(unsigned int eventID, float deltaTime) override;
		void OnSubjectDestroy() override;

	private:	
		// Member variables
		// ----------------
		std::shared_ptr<dae::Texture2D> m_pPookaWalkingAnimation{ nullptr };
		std::shared_ptr<dae::Texture2D> m_pFygarWalkingAnimation{ nullptr };

		std::unique_ptr<dae::MoveCommand> m_pMoveCommand{ nullptr };

		bool m_CommandInitialized{ false };

		grid::Cell* m_pPreviousCell{ nullptr };
		grid::Cell* m_pNextCell{ nullptr };

		float m_CurrentRoamTime{};
		float m_CurrentAttackTime{};

		EnemyComponent* m_pEnemyComponent{ nullptr };
		bool m_WantedToAttack{ false };
		bool m_WantedToGhost{ false };

		// Member functions
		// ----------------
		void InitMovementCommands(EnemyComponent* pEnemy);

		EnemyStates CheckGhostTimer(EnemyComponent* pEnemy, float deltaTime);
		EnemyStates HandleAttack(EnemyComponent* pEnemy, float deltaTime);
		void HandlePathing(EnemyComponent* pEnemy, float deltaTime);
		void FindNextCell(grid::Cell* pCurrentCell);

		EnemyStates LookForPlayer(EnemyComponent* pEnemy, float deltaTime);
		bool IsDirtBetween(int cellsBetween, bool checkRows, grid::GridComponent* pGrid, grid::Cell* pCurrentCell, grid::Cell* pCharacterCell);

		void Unsubscribe(EnemyComponent* pEnemy);
	};
}