#include "ChaseState.h"

void Enemy::ChaseState::OnEnter(EnemyComponent* /*pEnemy*/)
{

}
void Enemy::ChaseState::OnLeave(EnemyComponent* /*pEnemy*/)
{

}

Enemy::EnemyStates Enemy::ChaseState::Update(EnemyComponent* /*pEnemy*/, float /*deltaTime*/)
{
	return NR_STATES;
}