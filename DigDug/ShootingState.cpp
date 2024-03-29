#include "ShootingState.h"

#include "CharacterComponent.h"
#include "AnimationComponent.h"
#include "RenderTextureComponent.h"
#include "RopeComponent.h"
#include "GameObject.h"

#include "DigDugSceneManager.h"

#include "EventsEnum.h"
#include "ServiceLocator.h"
#include "ResourceManager.h"

player::ShootingState::ShootingState()
{
	// Player texture
	std::string textureString{ "Sprites/Characters/MainCharacter/Shoot_Stance.png" };
	m_pShootingSprite = dae::ResourceManager::GetInstance().LoadTexture(textureString);	

	// SFX ID
	const std::string fileName{ "Sound/Characters/Player/Shoot.wav" };
	dae::ServiceLocator::GetSoundSystem().SetID(event::PlayerShoot, fileName);
}

void player::ShootingState::OnEnter(CharacterComponent* pPlayer)
{
	// Set playerTexture
	// -----------------
	auto pAnimationComponent{ pPlayer->GetAnimationComponent() };
	pAnimationComponent->SetTexture(m_pShootingSprite);
	
	pAnimationComponent->SetSingleSpriteSize(25.f);
	pAnimationComponent->SetMaxFrames(1);
	pAnimationComponent->SetPaused(true);

	// Play SFX
	// --------
	const int volume{ 100 };
	const int loops{ 0 };
	dae::ServiceLocator::GetSoundSystem().PlayAudio(event::PlayerShoot, volume, loops);

	// Rope
	// ----

	// Init rope
	if (m_RopeInitialized == false)
	{
		m_RopeInitialized = true;
		InitRope(pPlayer);
	}

	// Set rope active
	auto pRope{ m_pRope->GetGameObject() };
	pRope->SetIsActive(true);
	pRope->SetIsHidden(false);

	// Rotate and transform accordingly
	auto pRopeTexture{ pRope->GetComponent<dae::RenderTextureComponent>() };
	const int ropeLength{ pRopeTexture->GetTextureSize().x };
	const float playerWidth{ pPlayer->GetAnimationComponent()->GetTextureSize().x / 2.f };
	const float offset{ playerWidth + ropeLength / 2.f };

	const player::LookingDirection lookingDirection{ pPlayer->GetLookingDirection() };
	switch (lookingDirection)
	{
	case player::Up:
		pRope->SetLocalPosition(0.f, -offset, 0.f);
		pRopeTexture->SetFlip(false);
		break;

	case player::Down:
		pRope->SetLocalPosition(0.f, offset, 0.f);
		pRopeTexture->SetFlip(true);
		break;

	case player::Left:
		pRope->SetLocalPosition(-offset, 0.f, 0.f);
		pRopeTexture->SetFlip(false);
		break;

	case player::Right:
		pRope->SetLocalPosition(offset, 0.f, 0.f);
		pRopeTexture->SetFlip(true);
		break;
	}

	// Throw
	m_pRope->StartThrow(pPlayer);
}
void player::ShootingState::OnLeave(CharacterComponent* /*pPlayer*/)
{
	// Set rope in-active
	auto pRope{ m_pRope->GetGameObject() };
	pRope->SetIsActive(false);
	pRope->SetIsHidden(true);
}

player::PlayerStates player::ShootingState::Update(CharacterComponent* /*pPlayer*/, float /*deltaTime*/)
{
	// Check if rope is done throwing
	player::PlayerStates state{ NR_STATES };
	if (m_pRope->GetIsThrowing() == false)
	{
		if (m_pRope->GetCaughtEnemy()) state = player::Blowing;
		else						   state = player::Digging;
	}

	// Return
	return state;
}

void player::ShootingState::InitRope(CharacterComponent* pPlayer)
{
	auto pGrid{ digdug::DigDugSceneManager::GetInstance().GetGrid() };

	// Create gameObject
	std::shared_ptr<dae::GameObject> pRope{ std::make_shared<dae::GameObject>() };

	// Add texture
	std::string textureString{ "Sprites/Characters/MainCharacter/Rope.png" };
	auto ropeSprite{ dae::ResourceManager::GetInstance().LoadTexture(textureString) };

	dae::RenderTextureComponent* pObjectTexture{ pRope->AddComponent<dae::RenderTextureComponent>() };
	pObjectTexture->SetTexture(ropeSprite);
	pObjectTexture->CenterTexture(true);

	// Add ropeComponent
	const float throwSpeed{ 500.f };
	
	m_pRope = pRope->AddComponent<RopeComponent>();
	m_pRope->SetThrowSpeed(throwSpeed);

	m_pRope->SetRenderTextureComponent(pObjectTexture);
	m_pRope->SetGrid(pGrid);

	// Add as child
	pRope->SetParent(pPlayer->GetGameObject(), false);
}