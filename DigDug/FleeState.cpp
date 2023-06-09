#include "FleeState.h"

#include "EnemyComponent.h"

#include "FirstScene.h"

void enemy::FleeState::OnEnter(EnemyComponent* pEnemy)
{
	if (m_pMoveCommand == nullptr)
	{
		const auto behaviorData{ pEnemy->GetBehaviorData() };
		m_pMoveCommand = std::make_unique<dae::MoveCommand>(pEnemy->GetGameObject(), glm::vec2{}, behaviorData.movementSpeed, FirstScene::GetInstance().GetGrid());
	}
}
void enemy::FleeState::OnLeave(EnemyComponent* /*pEnemy*/)
{
}

enemy::EnemyStates enemy::FleeState::Update(EnemyComponent* /*pEnemy*/, float /*deltaTime*/)
{
	return NR_STATES;
}