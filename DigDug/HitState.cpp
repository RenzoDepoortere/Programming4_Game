#include "HitState.h"

#include "CharacterComponent.h"
#include "AnimationComponent.h"
#include "GameObject.h"

#include "ResourceManager.h"
#include "EventManager.h"
#include "EventsEnum.h"
#include "ServiceLocator.h"

player::HitState::HitState()
{
	// Create textures
	m_pHitTexture = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Characters/MainCharacter/Dying_Animation.png");

	// Create SFX
	const std::string fileName{ "Sound/Characters/Player/Hit.wav" };
	dae::ServiceLocator::GetSoundSystem().SetID(event::PlayerHit, fileName);
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
	m_pAnimationComponent->SetFramesPerSecond(8);

	m_pAnimationComponent->SetPaused(false);
	m_pAnimationComponent->SetFlip(false);

	// Play SFX
	// --------
	const int volume{ 100 };
	const int loops{ 0 };
	dae::ServiceLocator::GetSoundSystem().PlayAudio(event::PlayerHit, volume, loops);
}
void player::HitState::OnLeave(CharacterComponent* /*pPlayer*/)
{
}

player::PlayerStates player::HitState::Update(CharacterComponent* pPlayer, float /*deltaTime*/)
{
	if (m_pAnimationComponent->PlayedOnce() && m_pAnimationComponent->GetPaused() == false)
	{
		pPlayer->GetGameObject()->SetIsActive(false);
	}

	return NR_STATES;
}