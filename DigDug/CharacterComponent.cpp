#include "CharacterComponent.h"

#include "GridComponent.h"
#include "AnimationComponent.h"
#include "GameObject.h"

#include "InputMapper.h"

CharacterComponent::CharacterComponent(dae::GameObject* pParentObject)
	: Component{ pParentObject }
{
}

void CharacterComponent::Update(float deltaTime)
{
	HandleInput(deltaTime, m_ControllerID);
	RemoveDirt();
}

void CharacterComponent::HandleInput(float deltaTime, unsigned long /*controlledID*/)
{
	// Init move command if necessary
	// ------------------------------
	if (m_MoveCommandInitialized == false)
	{
		m_MoveCommandInitialized = true;
		InitMoveCommand();
	}

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
	if (upPressed)
	{
		GetGameObject()->SetRotation(90.f);
		m_pAnimationComponent->SetFlip(true);
	}
	else if (downPressed)
	{
		GetGameObject()->SetRotation(90.f);
		m_pAnimationComponent->SetFlip(false);
	}
	else if (leftPressed)
	{
		GetGameObject()->SetRotation(0.f);
		m_pAnimationComponent->SetFlip(true);
	}
	else if (rightPressed)
	{
		GetGameObject()->SetRotation(0.f);
		m_pAnimationComponent->SetFlip(false);
	}

	// Play animation
	// --------------
	const bool isPaused{ m_pAnimationComponent->GetPaused() };

	// If input and paused
	if (isInput && isPaused)
	{
		// Play
		m_pAnimationComponent->SetPaused(false);

	}
	// If no input and playing
	if (isInput == false && isPaused == false)
	{
		// Pause
		m_pAnimationComponent->SetPaused(true);
	}
}
void CharacterComponent::RemoveDirt()
{
	if (m_pGrid == nullptr) return;

	// Get currentCell
	const glm::vec3 objectPos{ GetGameObject()->GetWorldPosition() };
	grid::Cell* pCurrentCell{ m_pGrid->GetCell(objectPos) };

	// Remove texture and add connection
	pCurrentCell->textureID = 0;
	m_pGrid->AddConnections(pCurrentCell);
}

void CharacterComponent::InitMoveCommand()
{
	// Create movementCommand
	// ----------------------
	float movementSpeed{ 100.f };
	glm::vec2 movementDirection{};

	m_pMoveCommand = std::make_unique<dae::MoveCommand>(GetGameObject(), movementDirection, movementSpeed, m_pGrid);
}