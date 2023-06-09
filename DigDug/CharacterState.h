#pragma once
class CharacterComponent;

namespace player
{
	enum PlayerStates
	{
		Digging,
		Shooting,
		Blowing,
		Squashed,
		Hit,
		NR_STATES
	};

	class CharacterState
	{
	public:
		// Rule of five
		CharacterState() = default;
		virtual ~CharacterState() = default;

		CharacterState(const CharacterState& other) = delete;
		CharacterState(CharacterState&& other) = delete;
		CharacterState& operator=(const CharacterState& other) = delete;
		CharacterState& operator=(CharacterState&& other) = delete;

		// Functionality
		virtual void OnEnter(CharacterComponent* pPlayer) = 0;
		virtual void OnLeave(CharacterComponent* pPlayer) = 0;

		virtual PlayerStates Update(CharacterComponent* pPlayer, float deltaTime) = 0;
	};
}