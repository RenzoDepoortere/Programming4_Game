#include "DiggingState.h"

#include "CharacterComponent.h"
#include "GameObject.h"
#include "GridComponent.h"
#include "AnimationComponent.h"

#include "InputMapper.h"

void Player::DiggingState::OnEnter(CharacterComponent* pPlayer)
{
	// Create movementCommand
	if (m_pMoveCommand == nullptr)
	{
		float movementSpeed{ 100.f };
		glm::vec2 movementDirection{};

		m_pMoveCommand = std::make_unique<dae::MoveCommand>(pPlayer->GetGameObject(), movementDirection, movementSpeed, pPlayer->GetGrid());
	}
}
void Player::DiggingState::OnLeave(CharacterComponent* /*pPlayer*/)
{
}

Player::PlayerStates Player::DiggingState::Update(CharacterComponent* pPlayer, float deltaTime)
{
	// Update
	PlayerStates state{};
	state = HandleInput(pPlayer, deltaTime);
	RemoveDirt(pPlayer);

	// Return
	return state;
}

Player::PlayerStates Player::DiggingState::HandleInput(CharacterComponent* pPlayer, float deltaTime)
{
	// Check input
	// -----------
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
	auto pObject{ pPlayer->GetGameObject() };
	auto pAnimationComponent{ pPlayer->GetAnimationComponent() };

	if (upPressed)
	{
		pObject->SetRotation(90.f);
		pAnimationComponent->SetFlip(true);
	}
	else if (downPressed)
	{
		pObject->SetRotation(90.f);
		pAnimationComponent->SetFlip(false);
	}
	else if (leftPressed)
	{
		pObject->SetRotation(0.f);
		pAnimationComponent->SetFlip(true);
	}
	else if (rightPressed)
	{
		pObject->SetRotation(0.f);
		pAnimationComponent->SetFlip(false);
	}

	// Play animation
	// --------------
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
void Player::DiggingState::RemoveDirt(CharacterComponent* pPlayer)
{
	// Get currentCell
	grid::GridComponent* pGrid{ pPlayer->GetGrid() };
	const glm::vec3 objectPos{ pPlayer->GetGameObject()->GetWorldPosition() };
	grid::Cell* pCurrentCell{ pGrid->GetCell(objectPos) };

	// Remove texture and add connection
	pCurrentCell->textureID = 0;
	pGrid->AddConnections(pCurrentCell);
}