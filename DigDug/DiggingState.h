#pragma once
#include "CharacterState.h"
#include "MoveCommand.h"
#include "Texture2D.h"
#include "Observer.h"

#include <memory>

namespace player
{
	class DiggingState final : public player::CharacterState, public dae::Observer<float>
	{
	public:
		// Rule of Five
		DiggingState();
		virtual ~DiggingState() override = default;

		DiggingState(const DiggingState& other) = delete;
		DiggingState(DiggingState&& other) = delete;
		DiggingState& operator=(const DiggingState& other) = delete;
		DiggingState& operator=(DiggingState&& other) = delete;

		// Functionality
		virtual void OnEnter(CharacterComponent* pPlayer) override;
		virtual void OnLeave(CharacterComponent* pPlayer) override;

		virtual PlayerStates Update(CharacterComponent* pPlayer, float deltaTime) override;

		// Observer
		virtual void HandleEvent(unsigned int eventID, float deltaTime) override;
		virtual void OnSubjectDestroy() override;

	private:
		// Member variables
		// ----------------
		std::unique_ptr<dae::MoveCommand> m_pMoveCommand{ nullptr };
		std::shared_ptr<dae::Texture2D> m_pWalkingSprite{ nullptr };

		CharacterComponent* m_pCharacterComponent{ nullptr };
		bool m_IsWalking{ false };
		bool m_WasWalking{ false };
		bool m_WantsToShoot{ false };

		// Member functions
		// ----------------
		void HandleWalkingToggle(CharacterComponent* pPlayer);
		void RemoveDirt(CharacterComponent* pPlayer);
	};
}