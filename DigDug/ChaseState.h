#pragma once
#include "EnemyState.h"
#include "MoveCommand.h"

#include <memory>
#include <array>
#include <deque>

class EnemyComponent;
class CharacterComponent;

namespace grid
{
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
		// Structs
		// -------
		struct CellRecord
		{
			grid::Cell* pCell{ nullptr };
			grid::Cell* pPreviousCell{ nullptr };
			float costSoFar{ 0.f }; // accumulated g-costs of all the connections leading up to this one
			float estimatedTotalCost{ 0.f }; // f-cost (= costSoFar + h-cost)

			bool operator==(const CellRecord& other) const
			{
				return pCell == other.pCell
					&& pPreviousCell == other.pPreviousCell
					&& costSoFar == other.costSoFar
					&& estimatedTotalCost == other.estimatedTotalCost;
			};

			bool operator<(const CellRecord& other) const
			{
				return estimatedTotalCost < other.estimatedTotalCost;
			};
		};

		// Member variables
		// ----------------
		CharacterComponent* m_pCharacterToChase{ nullptr };
		std::unique_ptr<dae::MoveCommand> m_pMoveCommand{ nullptr };

		std::deque<grid::Cell*> m_DesiredPath{};
		float m_CheckInterval{};	// Each X seconds, the AI will recalculate it's path
		float m_CurrentTime{};

		// Member functions
		// ----------------
		std::deque<grid::Cell*> CalculatePath(EnemyComponent* pEnemy);
		void FollowPath(EnemyComponent* pEnemy, float deltaTime);

		float GetHeuristicCost(grid::Cell* pStartNode, grid::Cell* pEndNode) const;
	};
}