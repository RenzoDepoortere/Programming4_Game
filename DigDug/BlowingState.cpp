#include "BlowingState.h"

player::BlowingState::BlowingState()
{
	// Create blow textures
}

void player::BlowingState::OnEnter(CharacterComponent* /*pPlayer*/)
{
	// Set sprite to blowing
}
void player::BlowingState::OnLeave(CharacterComponent* /*pPlayer*/)
{
}

player::PlayerStates player::BlowingState::Update(CharacterComponent* /*pPlayer*/, float /*deltaTime*/)
{
	return NR_STATES;
}