#include "RockComponent.h"
#include "GameObject.h"
#include "GridComponent.h"
#include "RenderTextureComponent.h"
#include "ServiceLocator.h"
#include "EventManager.h"
#include "EventsEnum.h"

RockComponent::RockComponent(dae::GameObject* pParentObject)
	: Component{ pParentObject }
{
	// Create move command
	const glm::vec2 downDirection{ 0, 1 };
	const float movementDirection{ 150.f };

	pMoveCommand = std::make_unique<dae::MoveCommand>(GetGameObject(), downDirection, movementDirection);

	// Get SFX ID
	const std::string fileName{ "Sound/FallenRock_SFX.wav" };
	dae::ServiceLocator::GetSoundSystem().SetID(event::RockBreak, fileName);

	// Get animationComponent
}

void RockComponent::Update(float deltaTime)
{
	if (m_pGrid == nullptr) return;

	switch (m_CurrentRockState)
	{
	case RockComponent::Passive:
		BottomCellGone();
		break;

	case RockComponent::Unstable:
		Rumble(deltaTime);
		break;

	case RockComponent::Falling:
		Fall(deltaTime);
		break;

	case RockComponent::Destroyed:
		Destroy(deltaTime);
		break;
	}
}

void RockComponent::BottomCellGone()
{
	// If cell below is destroyed
	if (GetCellBelow()->textureID == 0)			// It can be assured that the cell below a rock is never null
	{
		// Current state is unstable
		m_CurrentRockState = Unstable;
		
		// Set rumbleTime
		const float rumbleTime{ 2.f };
		m_CurrentRumbleTime = rumbleTime;
	}
}
void RockComponent::Rumble(float deltaTime)
{
	// Check which direction to rotate in
	const float currentRotation{ GetGameObject()->GetRotation() };
	const float angleMax{ 30.0f };
	if (angleMax < abs(currentRotation))
	{
		m_RotationDirection *= -1;
	}

	// Calculate rotation
	const float rotationSpeed{ 200.0f };
	const float newRotation{ currentRotation + rotationSpeed * m_RotationDirection * deltaTime };

	// Set rotation
	GetGameObject()->SetRotation(newRotation);

	// Countdown
	m_CurrentRumbleTime -= deltaTime;
	if (m_CurrentRumbleTime <= 0)
	{
		// Current state is falling
		m_CurrentRockState = Falling;

		// Reset rotation
		GetGameObject()->SetRotation(0.f);

		// Free currentCell
		const glm::vec3 currentPos{ GetGameObject()->GetWorldPosition() };
		grid::Cell* pCurrentCell{ m_pGrid->GetCell(currentPos) };
		pCurrentCell->containsRock = false;
	}
}
void RockComponent::Fall(float deltaTime)
{
	// Move
	// ----
	pMoveCommand->Execute(deltaTime);

	// Check if cell below is blocked
	// ------------------------------
	const glm::ivec2 textureSize{ m_pRenderTextureComponent->GetTextureSize() };
	const glm::vec3 currentPos{ GetGameObject()->GetWorldPosition() };

	grid::Cell* pCurrentCell{ m_pGrid->GetCell(currentPos) };
	grid::Cell* pCellBelow{ GetCellBelow() };

	const float bottomY{ currentPos.y + textureSize.y / 2.f };
	bool canTransitionState{ false };

	// Check if is colliding
	// ---------------------
	// Todo: this could get converted into a collision system
	if (pCellBelow)
	{
		if (pCellBelow->textureID == 0) return;
		canTransitionState = pCellBelow->worldPosition.y <= bottomY && bottomY <= pCellBelow->worldPosition.y + pCellBelow->size.y;
	}
	else
	{
		canTransitionState = pCurrentCell->worldPosition.y + pCurrentCell->size.y < bottomY;
	}

	// Change state
	// ------------
	if (canTransitionState)
	{
		// Current state is destroyed
		m_CurrentRockState = Destroyed;

		// Set destroyTime
		const float destroyTime{ 1.f };
		m_CurrentDestroyTime = destroyTime;

		// Play SFX
		const int volume{ 100 };
		const int loops{ 0 };

		dae::ServiceLocator::GetSoundSystem().PlayAudio(event::RockBreak, volume, loops);
	}
}
void RockComponent::Destroy(float deltaTime)
{
	// Animation

	// Countdown
	m_CurrentDestroyTime -= deltaTime;
	if (m_CurrentDestroyTime < 0)
	{
		// Destroy object
		GetGameObject()->RemoveObject();
	}
}

grid::Cell* RockComponent::GetCellBelow() const
{
	// Get currentCell
	const glm::vec3 currentPos{ GetGameObject()->GetWorldPosition() };
	grid::Cell* pCurrentCell{ m_pGrid->GetCell(currentPos) };

	// Get cellBelow
	glm::vec3 bottomCellPos{ currentPos };
	bottomCellPos.y += pCurrentCell->size.y;

	return m_pGrid->GetCell(bottomCellPos);
}