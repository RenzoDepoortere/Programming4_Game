#include "BlowingState.h"

#include "CharacterComponent.h"
#include "AnimationComponent.h"
#include "EnemyComponent.h"
#include "GameObject.h"

#include "ResourceManager.h"
#include "InputManager.h"
#include "EventsEnum.h"
#include "EventManager.h"
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
	// Reset variables
	// ---------------
	m_CurrentTime = 0.f;
	m_PumpSprite = false;
	m_WantsToDig = false;
	m_CanBlow = true;

	m_pCharacterComponent = pPlayer;

	// Set sprite to blowing
	// ---------------------
	m_pAnimationComponent = pPlayer->GetAnimationComponent();
	m_pAnimationComponent->SetTexture(m_pBlowingSprite);

	m_pAnimationComponent->SetSingleSpriteSize(26.f);
	m_pAnimationComponent->SetMaxFrames(2);
	m_pAnimationComponent->SetPaused(true);

	// Subscribe to events
	// -------------------

	// If player 1
	if (pPlayer->GetPlayerID() == 0)
	{
		dae::EventManager<float>::GetInstance().Subscribe(event::KeyboardLeft, this);
		dae::EventManager<float>::GetInstance().Subscribe(event::KeyboardRight, this);
		dae::EventManager<float>::GetInstance().Subscribe(event::KeyboardUp, this);
		dae::EventManager<float>::GetInstance().Subscribe(event::KeyboardDown, this);

		dae::EventManager<float>::GetInstance().Subscribe(event::KeyboardActionA, this);

		dae::EventManager<float>::GetInstance().Subscribe(event::ControllerLeft_2, this);
		dae::EventManager<float>::GetInstance().Subscribe(event::ControllerRight_2, this);
		dae::EventManager<float>::GetInstance().Subscribe(event::ControllerUp_2, this);
		dae::EventManager<float>::GetInstance().Subscribe(event::ControllerDown_2, this);

		dae::EventManager<float>::GetInstance().Subscribe(event::ControllerActionA_2, this);
	}
	// If player 2
	else
	{
		dae::EventManager<float>::GetInstance().Subscribe(event::ControllerLeft_1, this);
		dae::EventManager<float>::GetInstance().Subscribe(event::ControllerRight_1, this);
		dae::EventManager<float>::GetInstance().Subscribe(event::ControllerUp_1, this);
		dae::EventManager<float>::GetInstance().Subscribe(event::ControllerDown_1, this);

		dae::EventManager<float>::GetInstance().Subscribe(event::ControllerActionA_1, this);
	}
}
void player::BlowingState::OnLeave(CharacterComponent* pPlayer)
{
	// Unsubscribe to events
	// ---------------------
	if (dae::EventManager<float>::GetIsDestroyed() == false)
	{
		// If player 1
		if (pPlayer->GetPlayerID() == 0)
		{
			dae::EventManager<float>::GetInstance().Unsubscribe(event::KeyboardLeft, this);
			dae::EventManager<float>::GetInstance().Unsubscribe(event::KeyboardRight, this);
			dae::EventManager<float>::GetInstance().Unsubscribe(event::KeyboardUp, this);
			dae::EventManager<float>::GetInstance().Unsubscribe(event::KeyboardDown, this);

			dae::EventManager<float>::GetInstance().Unsubscribe(event::KeyboardActionA, this);

			dae::EventManager<float>::GetInstance().Unsubscribe(event::ControllerLeft_2, this);
			dae::EventManager<float>::GetInstance().Unsubscribe(event::ControllerRight_2, this);
			dae::EventManager<float>::GetInstance().Unsubscribe(event::ControllerUp_2, this);
			dae::EventManager<float>::GetInstance().Unsubscribe(event::ControllerDown_2, this);

			dae::EventManager<float>::GetInstance().Unsubscribe(event::ControllerActionA_2, this);
		}
		// If player 2
		else
		{
			dae::EventManager<float>::GetInstance().Unsubscribe(event::ControllerLeft_1, this);
			dae::EventManager<float>::GetInstance().Unsubscribe(event::ControllerRight_1, this);
			dae::EventManager<float>::GetInstance().Unsubscribe(event::ControllerUp_1, this);
			dae::EventManager<float>::GetInstance().Unsubscribe(event::ControllerDown_1, this);

			dae::EventManager<float>::GetInstance().Unsubscribe(event::ControllerActionA_1, this);
		}
	}
}

player::PlayerStates player::BlowingState::Update(CharacterComponent* pPlayer, float deltaTime)
{
	// Cooldown
	HandleCooldown(deltaTime);

	// Check if enemy dead
	PlayerStates state{};
	state = CheckEnemy(pPlayer);
	if (state != NR_STATES) return state;

	// Check if can wants to go back
	if (m_WantsToDig)
	{
		// Release enemy and go back to digging
		EnemyComponent* pCaughtEnemy{ pPlayer->GetCaughtEnemy() };
		pCaughtEnemy->SetCaught(false);

		state = Digging;
	}

	// Return
	return state;
}

void player::BlowingState::HandleEvent(unsigned int eventID, float /*deltaTime*/)
{
	// Check event
	// -----------
	bool wasInput{ false };

	switch (eventID)
	{
	case event::KeyboardLeft:
	case event::ControllerLeft_1:
	case event::ControllerLeft_2:
		wasInput = true;
		break;

	case event::KeyboardRight:
	case event::ControllerRight_1:
	case event::ControllerRight_2:
		wasInput = true;
		break;

	case event::KeyboardUp:
	case event::ControllerUp_1:
	case event::ControllerUp_2:
		wasInput = true;
		break;

	case event::KeyboardDown:
	case event::ControllerDown_1:
	case event::ControllerDown_2:
		wasInput = true;
		break;

	case event::KeyboardActionA:
	case event::ControllerActionA_1:
	case event::ControllerActionA_2:
		Blow();
		break;
	}

	// Set variables
	// -------------
	m_WantsToDig = wasInput;
}
void player::BlowingState::OnSubjectDestroy()
{
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
void player::BlowingState::HandleCooldown(float deltaTime)
{
	if (m_CanBlow) return;

	m_CurrentTime += deltaTime;
	const float threshold{ 0.5f };
	if (threshold <= m_CurrentTime)
	{
		m_CurrentTime = 0.f;
		m_CanBlow = true;
	}
}

void player::BlowingState::Blow()
{
	// Return if can't blow
	if (m_CanBlow == false) return;

	// Give blow to caught Enemy
	EnemyComponent* pCaughtEnemy{ m_pCharacterComponent->GetCaughtEnemy() };
	pCaughtEnemy->SetBlow(true);

	// Change Animation sprite
	m_PumpSprite = !m_PumpSprite;
	m_pAnimationComponent->SetFrame(static_cast<int>(m_PumpSprite));

	// Play SFX
	const int volume{ 100 };
	const int loops{ 0 };
	dae::ServiceLocator::GetSoundSystem().PlayAudio(event::PlayerBlow, volume, loops);

	// Set cooldown
	m_CanBlow = false;
	m_CurrentTime = 0.f;
}