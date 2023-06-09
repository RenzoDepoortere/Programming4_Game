#include "AttackState.h"

#include "EnemyComponent.h"
#include "AnimationComponent.h"

#include "ResourceManager.h"

enemy::AttackState::AttackState()
{
	// Create textures
	m_pFygarFireTexture = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Characters/Enemies/Fygar/Fire.png");
}

void enemy::AttackState::OnEnter(EnemyComponent* pEnemy)
{
	// Set texture
	m_pAnimationComponent = pEnemy->GetAnimationComponent();
	m_pAnimationComponent->SetPaused(true);
}

void enemy::AttackState::OnLeave(EnemyComponent* /*pEnemy*/)
{
}

enemy::EnemyStates enemy::AttackState::Update(EnemyComponent* /*pEnemy*/, float /*deltaTime*/)
{
	return NR_STATES;
}