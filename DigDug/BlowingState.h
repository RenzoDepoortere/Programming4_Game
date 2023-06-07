#pragma once
#include "CharacterState.h"

namespace player
{
	class BlowingState final : public CharacterState
	{
	public:
		// Rule of Five
		BlowingState();
		virtual ~BlowingState() override = default;

		BlowingState(const BlowingState& other) = delete;
		BlowingState(BlowingState&& other) = delete;
		BlowingState& operator=(const BlowingState& other) = delete;
		BlowingState& operator=(BlowingState&& other) = delete;

		// Functionality
		virtual void OnEnter(CharacterComponent* pPlayer) override;
		virtual void OnLeave(CharacterComponent* pPlayer) override;

		virtual PlayerStates Update(CharacterComponent* pPlayer, float deltaTime) override;

	private:
		// Member variables
		// ----------------

		// Member functions
		// ----------------
	};
}