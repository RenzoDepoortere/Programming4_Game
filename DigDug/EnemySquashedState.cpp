#include "EnemySquashedState.h"

#include "EnemyComponent.h"
#include "AnimationComponent.h"

#include "ResourceManager.h"

enemy::EnemySquashedState::EnemySquashedState()
{
	// Create textures
	m_pPookaSquashedTexture = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Characters/Enemies/Pooka/Squashed_State.png");
	m_pFygarSquashedTexture = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Characters/Enemies/Fygar/Squashed_State.png");
}

void enemy::EnemySquashedState::OnEnter(EnemyComponent* pEnemy)
{
	// Set texture
	auto pAnimationComponent = pEnemy->GetAnimationComponent();

	auto pTexture = m_pPookaSquashedTexture;
	float textureSize = 21.f;
	if (pEnemy->GetBehaviorData().enemyType != Pooka)
	{
		pTexture = m_pFygarSquashedTexture;
	}

	pAnimationComponent->SetTexture(pTexture);
	pAnimationComponent->SetSingleSpriteSize(textureSize);

	pAnimationComponent->SetMaxFrames(1);
	pAnimationComponent->SetFramesPerSecond(12);

	pAnimationComponent->SetPaused(true);
}
void enemy::EnemySquashedState::OnLeave(EnemyComponent* /*pEnemy*/)
{
}

enemy::EnemyStates enemy::EnemySquashedState::Update(EnemyComponent* /*pEnemy*/, float /*deltaTime*/)
{
	return NR_STATES;
}