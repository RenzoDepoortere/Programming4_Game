#include "CaughtState.h"

enemy::CaughtState::CaughtState()
{
	// Create caught texture
}

void enemy::CaughtState::OnEnter(EnemyComponent* /*pEnemy*/)
{
	// Set caught texture
}
void enemy::CaughtState::OnLeave(EnemyComponent* /*pEnemy*/)
{
}

enemy::EnemyStates enemy::CaughtState::Update(EnemyComponent* /*pEnemy*/, float /*deltaTime*/)
{
	return NR_STATES;
}