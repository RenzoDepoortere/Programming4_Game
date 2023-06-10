#include "DiggingState.h"

#include "CharacterComponent.h"
#include "GameObject.h"
#include "GridComponent.h"
#include "AnimationComponent.h"

#include "DigDugSceneManager.h"

#include "InputMapper.h"
#include "ResourceManager.h"

player::DiggingState::DiggingState()
{
	std::string textureString{ "Sprites/Characters/MainCharacter/Walking_Animation.png" };
	m_pWalkingSprite = dae::ResourceManager::GetInstance().LoadTexture(textureString);
}

void player::DiggingState::OnEnter(CharacterComponent* pPlayer)
{
	// Create movementCommand
	if (m_pMoveCommand == nullptr)
	{
		float movementSpeed{ 100.f };
		glm::vec2 movementDirection{};

		m_pMoveCommand = std::make_unique<dae::MoveCommand>(pPlayer->GetGameObject(), movementDirection, movementSpeed, digdug::DigDugSceneManager::GetInstance().GetGrid(), false);
	}

	// Set playerTexture
	auto pAnimationComponent{ pPlayer->GetAnimationComponent() };
	pAnimationComponent->SetTexture(m_pWalkingSprite);

	pAnimationComponent->SetSingleSpriteSize(25.f);
	pAnimationComponent->SetMaxFrames(2);
	pAnimationComponent->SetPaused(false);
}
void player::DiggingState::OnLeave(CharacterComponent* /*pPlayer*/)
{
}

player::PlayerStates player::DiggingState::Update(CharacterComponent* pPlayer, float deltaTime)
{
	// Update
	PlayerStates state{};
	state = HandleInput(pPlayer, deltaTime);
	RemoveDirt(pPlayer);

	// Return
	return state;
}

player::PlayerStates player::DiggingState::HandleInput(CharacterComponent* pPlayer, float deltaTime)
{
	// Check input
	// -----------
	const bool actionKeyPressed{ dae::InputManager::GetInstance().IsPressed(SDL_SCANCODE_J) };
	if (actionKeyPressed) return Shooting;

	const bool upPressed{ dae::InputManager::GetInstance().IsDown(SDL_SCANCODE_W) };
	const bool downPressed{ dae::InputManager::GetInstance().IsDown(SDL_SCANCODE_S) };
	const bool leftPressed{ dae::InputManager::GetInstance().IsDown(SDL_SCANCODE_A) };
	const bool rightPressed{ dae::InputManager::GetInstance().IsDown(SDL_SCANCODE_D) };

	const bool isInput{ upPressed || downPressed || leftPressed || rightPressed };

	// Move
	// ----
	if (isInput)
	{
		glm::vec2 moveDirection{};
		if (upPressed) moveDirection = glm::vec2{ 0.f, -1.f };
		else if (downPressed) moveDirection = glm::vec2{ 0.f, 1.f };
		else if (leftPressed) moveDirection = glm::vec2{ -1.f, 0.f };
		else if (rightPressed) moveDirection = glm::vec2{ 1.f, 0.f };

		m_pMoveCommand->SetMovementDirection(moveDirection);
		m_pMoveCommand->Execute(deltaTime);
	}

	// Rotate accordingly
	// ------------------
	if (upPressed)			pPlayer->SetLookingDirection(Up);
	else if (downPressed)	pPlayer->SetLookingDirection(Down);
	else if (leftPressed)	pPlayer->SetLookingDirection(Left);
	else if (rightPressed)	pPlayer->SetLookingDirection(Right);

	// Play animation
	// --------------
	auto pAnimationComponent{ pPlayer->GetAnimationComponent() };
	const bool isPaused{ pAnimationComponent->GetPaused() };

	// If input and paused
	if (isInput && isPaused)
	{
		// Play
		pAnimationComponent->SetPaused(false);

	}
	// If no input and playing
	if (isInput == false && isPaused == false)
	{
		// Pause
		pAnimationComponent->SetPaused(true);
	}

	// Return
	return NR_STATES;
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