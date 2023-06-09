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

namespace enemy
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
		virtual void OnEnter(EnemyComponent* pEnemy) override;
		virtual void OnLeave(EnemyComponent* pEnemy) override;

		virtual EnemyStates Update(EnemyComponent* pEnemy, float deltaTime) override;

	private:
		// Member variables
		// ----------------
		CharacterComponent* m_pCharacterToChase{ nullptr };
		std::unique_ptr<dae::MoveCommand> m_pMoveCommand{ nullptr };

		std::deque<grid::Cell*> m_DesiredPath{};
		float m_CheckInterval{};	// Each X seconds, the AI will recalculate it's path
		float m_CurrentTime{};

		// Member functions
		// ----------------
		void FollowPath(EnemyComponent* pEnemy, float deltaTime);
	};
}