#include "HitState.h"

#include "CharacterComponent.h"
#include "AnimationComponent.h"
#include "GameObject.h"

#include "ResourceManager.h"

player::HitState::HitState()
{
	// Create textures
	m_pHitTexture = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Characters/MainCharacter/Dying_Animation.png");
}

void player::HitState::OnEnter(CharacterComponent* pPlayer)
{
	// Reset rotation
	// --------------
	pPlayer->GetGameObject()->SetRotation(0.f);

	// Set texture
	// -----------
	m_pAnimationComponent = pPlayer->GetAnimationComponent();

	m_pAnimationComponent->SetTexture(m_pHitTexture);
	m_pAnimationComponent->SetSingleSpriteSize(26.f);

	m_pAnimationComponent->SetMaxFrames(4);
	m_pAnimationComponent->SetFramesPerSecond(12);

	m_pAnimationComponent->SetPaused(false);
	m_pAnimationComponent->SetFlip(false);
}
void player::HitState::OnLeave(CharacterComponent* /*pPlayer*/)
{
}

player::PlayerStates player::HitState::Update(CharacterComponent* /*pPlayer*/, float /*deltaTime*/)
{
	return NR_STATES;
}