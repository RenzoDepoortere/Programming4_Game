#include "BlowingState.h"

#include "CharacterComponent.h"
#include "AnimationComponent.h"
#include "EnemyComponent.h"
#include "GameObject.h"

#include "ResourceManager.h"
#include "InputManager.h"
#include "EventsEnum.h"
#include "ServiceLocator.h"

#include <string>

player::BlowingState::BlowingState()
{
	// Create blow textures
	std::string textureString{ "Sprites/Characters/MainCharacter/Pump_Animation.png" };
	m_pBlowingSprite = dae::ResourceManager::GetInstance().LoadTexture(textureString);

	// SFX ID
	const std::string fileName{ "Sound/Characters/Player/Blow.wav" };
	dae::ServiceLocator::GetSoundSystem().SetID(event::PlayerBlow, fileName);
}

void player::BlowingState::OnEnter(CharacterComponent* pPlayer)
{
	// Set sprite to blowing
	m_pAnimationComponent = pPlayer->GetAnimationComponent();
	m_pAnimationComponent->SetTexture(m_pBlowingSprite);

	m_pAnimationComponent->SetSingleSpriteSize(26.f);
	m_pAnimationComponent->SetMaxFrames(2);
	m_pAnimationComponent->SetPaused(true);
}
void player::BlowingState::OnLeave(CharacterComponent* /*pPlayer*/)
{
}

player::PlayerStates player::BlowingState::Update(CharacterComponent* pPlayer, float /*deltaTime*/)
{
	PlayerStates state{};

	// First check if enemy dead
	state = CheckEnemy(pPlayer);
	if (state != NR_STATES) return state;

	// Then handle player input
	state = HandleInput(pPlayer);

	// Return
	return state;
}

player::PlayerStates player::BlowingState::CheckEnemy(CharacterComponent* pPlayer)
{
	const EnemyComponent* pCaughtEnemy{ pPlayer->GetCaughtEnemy() };
	const bool isActive{ pCaughtEnemy->GetGameObject()->GetIsActive() };
	const bool gotFree{ pCaughtEnemy->GetCaught() == false };

	// If enemy dead or free, return to diggingState
	if (isActive == false || gotFree) return Digging;
	else							  return NR_STATES;
}
player::PlayerStates player::BlowingState::HandleInput(CharacterComponent* pPlayer)
{
	EnemyComponent* pCaughtEnemy{ pPlayer->GetCaughtEnemy() };

	// Check movement input
	const bool upPressed{ dae::InputManager::GetInstance().IsDown(SDL_SCANCODE_W) };
	const bool downPressed{ dae::InputManager::GetInstance().IsDown(SDL_SCANCODE_S) };
	const bool leftPressed{ dae::InputManager::GetInstance().IsDown(SDL_SCANCODE_A) };
	const bool rightPressed{ dae::InputManager::GetInstance().IsDown(SDL_SCANCODE_D) };

	// If was input
	const bool isInput{ upPressed || downPressed || leftPressed || rightPressed };
	if (isInput)
	{
		// Release enemy and go back to digging
		pCaughtEnemy->SetCaught(false);
		return Digging;
	}

	// If no actionKey pressed, return
	const bool actionKeyPressed{ dae::InputManager::GetInstance().IsPressed(SDL_SCANCODE_J) };
	if (actionKeyPressed == false) return NR_STATES;

	// Give blow to caught Enemy
	pCaughtEnemy->SetBlow(true);

	// Change Animation sprite
	m_PumpSprite = !m_PumpSprite;
	m_pAnimationComponent->SetFrame(static_cast<int>(m_PumpSprite));

	// Play SFX
	const int volume{ 100 };
	const int loops{ 0 };
	dae::ServiceLocator::GetSoundSystem().PlayAudio(event::PlayerBlow, volume, loops);

	// Return
	return NR_STATES;
}