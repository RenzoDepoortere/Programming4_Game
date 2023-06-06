#include "EnemyComponent.h"

#include "GridComponent.h"

EnemyComponent::EnemyComponent(dae::GameObject* pParentObject)
	: Component{ pParentObject }
{
}

void EnemyComponent::Update(float deltaTime)
{
	// Update currentState
	//m_pCurrentState->Update(deltaTime);

	TempPathing(deltaTime);
}

void EnemyComponent::CreateMovementCommand()
{
	// Create move command
	const glm::vec2 direction{ 0, 1 };
	pMoveCommand = std::make_unique<dae::MoveCommand>(GetGameObject(), direction, m_EnemyBehavior.movementSpeed, m_pGrid, false);
}

void EnemyComponent::TempPathing(float /*deltaTime*/)
{

}