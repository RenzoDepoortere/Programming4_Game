#pragma once
#include "CharacterState.h"
#include "MoveCommand.h"

#include <memory>

namespace Player
{
	class DiggingState final : public Player::CharacterState
	{
	public:
		// Rule of Five
		DiggingState() = default;
		virtual ~DiggingState() override = default;

		DiggingState(const DiggingState& other) = delete;
		DiggingState(DiggingState&& other) = delete;
		DiggingState& operator=(const DiggingState& other) = delete;
		DiggingState& operator=(DiggingState&& other) = delete;

		// Functionality
		virtual PlayerStates Update(CharacterComponent* pPlayer, float deltaTime) override;

		virtual void OnEnter(CharacterComponent* pPlayer) override;
		virtual void OnLeave(CharacterComponent* pPlayer) override;

	private:
		// Member variables
		// ----------------
		std::unique_ptr<dae::MoveCommand> m_pMoveCommand{ nullptr };

		// Member functions
		// ----------------
		PlayerStates HandleInput(CharacterComponent* pPlayer, float deltaTime);
		void RemoveDirt(CharacterComponent* pPlayer);
	};
}