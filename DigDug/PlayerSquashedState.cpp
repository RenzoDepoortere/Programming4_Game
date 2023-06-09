#include "PlayerSquashedState.h"

#include "CharacterComponent.h"
#include "AnimationComponent.h"
#include "GameObject.h"

#include "ResourceManager.h"

player::PlayerSquashedState::PlayerSquashedState()
{
	// Create textures
	m_pSquashedTexture = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Characters/MainCharacter/Squashed_State.png");
}

void player::PlayerSquashedState::OnEnter(CharacterComponent* pEnemy)
{
	// Reset rotation
	// --------------
	pEnemy->GetGameObject()->SetRotation(0.f);

	// Set texture
	// -----------
	auto pAnimationComponent = pEnemy->GetAnimationComponent();

	pAnimationComponent->SetTexture(m_pSquashedTexture);
	pAnimationComponent->SetSingleSpriteSize(23.f);

	pAnimationComponent->SetMaxFrames(1);
	pAnimationComponent->SetFramesPerSecond(12);

	pAnimationComponent->SetPaused(true);
	pAnimationComponent->SetFlip(false);
}
void player::PlayerSquashedState::OnLeave(CharacterComponent* /*pPlayer*/)
{
}

player::PlayerStates player::PlayerSquashedState::Update(CharacterComponent* /*pPlayer*/, float /*deltaTime*/)
{
	return NR_STATES;
}