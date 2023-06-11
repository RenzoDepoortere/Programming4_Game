#include "DiggingState.h"

#include "CharacterComponent.h"
#include "GameObject.h"
#include "GridComponent.h"
#include "AnimationComponent.h"

#include "DigDugSceneManager.h"

#include "InputMapper.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "EventsEnum.h"
#include "EventManager.h"
#include "ServiceLocator.h"

player::DiggingState::DiggingState()
{
	// Textures
	std::string textureString{ "Sprites/Characters/MainCharacter/Walking_Animation.png" };
	m_pWalkingSprite = dae::ResourceManager::GetInstance().LoadTexture(textureString);

	// SFX
	auto& soundSystem{ dae::ServiceLocator::GetSoundSystem() };
	const std::string fileName{ "Sound/Other/ThemeSong.mp3" };
	const int volume{ 75 };

	soundSystem.SetID(event::PlayerWalking, fileName);
	soundSystem.PlayAudio(event::PlayerWalking, volume, -1, 4);
	soundSystem.PauseAudio(event::PlayerWalking, 4);
}
player::DiggingState::~DiggingState()
{
	// Unsubscribe
	Unsubscribe(m_pCharacterComponent);
}

void player::DiggingState::OnEnter(CharacterComponent* pPlayer)
{
	// Set variables
	// -------------
	m_pCharacterComponent = pPlayer;

	m_IsWalking = false;
	m_WasWalking = false;
	m_WantsToShoot = false;

	// Create movementCommand
	// ----------------------
	if (m_pMoveCommand == nullptr)
	{
		float movementSpeed{ 100.f };
		glm::vec2 movementDirection{};

		m_pMoveCommand = std::make_unique<dae::MoveCommand>(pPlayer->GetGameObject(), movementDirection, movementSpeed, digdug::DigDugSceneManager::GetInstance().GetGrid(), false);
	}

	// Set playerTexture
	// -----------------
	auto pAnimationComponent{ pPlayer->GetAnimationComponent() };
	pAnimationComponent->SetTexture(m_pWalkingSprite);

	pAnimationComponent->SetSingleSpriteSize(25.f);
	pAnimationComponent->SetMaxFrames(2);
	pAnimationComponent->SetPaused(true);

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
void player::DiggingState::OnLeave(CharacterComponent* pPlayer)
{
	// Stop music
	dae::ServiceLocator::GetSoundSystem().PauseAudio(event::PlayerWalking, 4);

	// Unsubscribe
	Unsubscribe(pPlayer);
}

player::PlayerStates player::DiggingState::Update(CharacterComponent* pPlayer, float /*deltaTime*/)
{
	// Handle walking
	HandleWalkingToggle(pPlayer);
	RemoveDirt(pPlayer);

	// Update state
	PlayerStates state{ NR_STATES };
	if (m_WantsToShoot) state = Shooting;

	// Return
	return state;
}
void player::DiggingState::HandleEvent(unsigned int eventID, float deltaTime)
{
	// Check event
	// -----------
	glm::vec2 movementDirection{};
	player::LookingDirection lookingDirection{ m_pCharacterComponent->GetLookingDirection() };
	bool wasInput{ false };

	switch (eventID)
	{
	case event::KeyboardLeft:
	case event::ControllerLeft_1:
	case event::ControllerLeft_2:
		movementDirection = glm::vec2{ -1.f, 0.f };
		lookingDirection = player::Left;

		wasInput = true;
		break;

	case event::KeyboardRight:
	case event::ControllerRight_1:
	case event::ControllerRight_2:
		movementDirection = glm::vec2{ 1.f, 0.f };
		lookingDirection = player::Right;

		wasInput = true;
		break;

	case event::KeyboardUp:
	case event::ControllerUp_1:
	case event::ControllerUp_2:
		movementDirection = glm::vec2{ 0.f, -1.f };
		lookingDirection = player::Up;

		wasInput = true;
		break;

	case event::KeyboardDown:
	case event::ControllerDown_1:
	case event::ControllerDown_2:
		movementDirection = glm::vec2{ 0.f, 1.f };
		lookingDirection = player::Down;

		wasInput = true;
		break;

	case event::KeyboardActionA:
	case event::ControllerActionA_1:
	case event::ControllerActionA_2:
		m_WantsToShoot = true;
		break;
	}

	// Set variables
	// -------------
	m_pCharacterComponent->SetLookingDirection(lookingDirection);
	m_IsWalking = true;

	// Handle Movement
	// ---------------
	m_pMoveCommand->SetMovementDirection(movementDirection);
	m_pMoveCommand->Execute(deltaTime);
}
void player::DiggingState::OnSubjectDestroy()
{
}

void player::DiggingState::HandleWalkingToggle(CharacterComponent* pPlayer)
{
	auto& soundSystem{ dae::ServiceLocator::GetSoundSystem() };
	const bool isMusicPaused{ soundSystem.IsPausedAudio(event::PlayerWalking, 4) };

	auto pAnimationComponent{ pPlayer->GetAnimationComponent() };
	const bool isAnimationPaused{ pAnimationComponent->GetPaused() };

	// On start walk
	// -------------
	if (m_IsWalking && m_WasWalking == false)
	{
		// Play animation if was paused
		if (isAnimationPaused) pAnimationComponent->SetPaused(false);

		// Resume music if was paused
		if (isMusicPaused) soundSystem.ResumeAudio(event::PlayerWalking, 4);
	}
	// On stop walk
	// ------------
	else if (m_IsWalking == false && m_WasWalking)
	{
		// Pause animation if was playing
		if (isAnimationPaused == false) pAnimationComponent->SetPaused(true);

		// Pause music if was playing
		if (isMusicPaused == false) soundSystem.PauseAudio(event::PlayerWalking, 4);
	}

	// Set variables
	m_WasWalking = m_IsWalking;
	m_IsWalking = false;
}
void player::DiggingState::RemoveDirt(CharacterComponent* pPlayer)
{
	// Get currentCell
	grid::GridComponent* pGrid{ digdug::DigDugSceneManager::GetInstance().GetGrid() };
	const glm::vec3 objectPos{ pPlayer->GetGameObject()->GetWorldPosition() };
	grid::Cell* pCurrentCell{ pGrid->GetCell(objectPos) };

	// Remove texture and add connection
	pCurrentCell->textureID = 0;
	pGrid->AddConnections(pCurrentCell);
}

void player::DiggingState::Unsubscribe(CharacterComponent* pPlayer)
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