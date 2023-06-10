#include "RockComponent.h"
#include "DigDugSceneManager.h"

#include "GameObject.h"
#include "GridComponent.h"
#include "AnimationComponent.h"
#include "EnemyComponent.h"
#include "CharacterComponent.h"

#include "ServiceLocator.h"
#include "EventManager.h"
#include "EventsEnum.h"
#include "Renderer.h"

RockComponent::RockComponent(dae::GameObject* pParentObject)
	: Component{ pParentObject }
{
	// Create move command
	const glm::vec2 downDirection{ 0, 1 };
	const float movementDirection{ 150.f };

	m_pMoveCommand = std::make_unique<dae::MoveCommand>(GetGameObject(), downDirection, movementDirection);

	// Get SFX ID
	const std::string fileName{ "Sound/Other/FallenRock_SFX.wav" };
	dae::ServiceLocator::GetSoundSystem().SetID(event::RockBreak, fileName);
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
void RockComponent::Render() const
{
	//const glm::vec3 worldPos{ GetGameObject()->GetWorldPosition() };
	//const utils::Rect boundingRect{ m_pAnimationComponent->GetBoundingRect() };

	//// Draw boundingRect
	//auto pRenderer{ dae::Renderer::GetInstance().GetSDLRenderer() };
	//SDL_SetRenderDrawColor(pRenderer, static_cast<Uint8>(0), static_cast<Uint8>(0), static_cast<Uint8>(255), static_cast<Uint8>(255));

	//SDL_Rect rect{};
	//rect.x = static_cast<int>(boundingRect.x);
	//rect.y = static_cast<int>(boundingRect.y);
	//rect.w = static_cast<int>(boundingRect.width);
	//rect.h = static_cast<int>(boundingRect.height);

	//SDL_RenderDrawRect(pRenderer, &rect);
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
	// Check if actors hit
	// -------------------
	const glm::ivec2 textureSize{ m_pAnimationComponent->GetTextureSize() };
	const utils::Rect ownBoundingRect{ m_pAnimationComponent->GetBoundingRect() };
	utils::Rect boundingRect{};

	dae::GameObject* pOwnGameObject{ GetGameObject() };
	auto pChildren{ pOwnGameObject->GetChildren() };

	dae::GameObject* pGameObject{ nullptr };

	// Enemies
	for (const auto& currentEnemy : digdug::DigDugSceneManager::GetInstance().GetEnemies())
	{
		// If in ghostState, continue
		if (currentEnemy->GetCurrentStateID() == enemy::Ghost) continue;

		// Check if can add
		pGameObject = currentEnemy->GetGameObject();
		boundingRect = currentEnemy->GetAnimationComponent()->GetBoundingRect();

		if (CheckIfCanAdd(ownBoundingRect, pOwnGameObject, boundingRect, pGameObject))
		{
			// Set to squashState
			currentEnemy->SetSquashed();
			m_HasBeenDestroyed.AddObserver(currentEnemy);
		}
	}

	// Player
	for (const auto& currentPlayer : digdug::DigDugSceneManager::GetInstance().GetCharacters())
	{
		// If in hitState, continue
		if (currentPlayer->GetCurrentStateID() == player::Hit) continue;

		// Check if can add
		pGameObject = currentPlayer->GetGameObject();
		boundingRect = currentPlayer->GetAnimationComponent()->GetBoundingRect();

		if (CheckIfCanAdd(ownBoundingRect, pOwnGameObject, boundingRect, pGameObject))
		{
			// Set to squashState
			currentPlayer->SetSquashed();
			m_HasBeenDestroyed.AddObserver(currentPlayer);
		}
	}

	// Move
	// ----
	m_pMoveCommand->Execute(deltaTime);

	// Check if cell below is blocked
	// ------------------------------
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

		// Play animation
		m_pAnimationComponent->SetPaused(false);

		// Play SFX
		const int volume{ 100 };
		const int loops{ 0 };

		dae::ServiceLocator::GetSoundSystem().PlayAudio(event::RockBreak, volume, loops);

		// Send event
		m_HasBeenDestroyed.Notify();
	}
}
void RockComponent::Destroy(float /*deltaTime*/)
{
	// Destroy if animation played
	if (m_pAnimationComponent->PlayedOnce())
	{
		// Destroy object
		GetGameObject()->SetIsActive(false);
		GetGameObject()->SetIsHidden(true);
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

bool RockComponent::CheckIfCanAdd(const utils::Rect& ownBoundingRect, dae::GameObject* pOwnGameObject, const utils::Rect& boundingRect, dae::GameObject* pGameObject)
{
	// Check if is not dead
	if (pGameObject->GetIsActive() == false) return false;

	// Check if not already child
	if (pGameObject->GetParent() == pOwnGameObject) return false;

	// Check if overlaps
	if (utils::RectOverlaps(ownBoundingRect, boundingRect))
	{
		// Add as child
		pGameObject->SetParent(pOwnGameObject, true);
		return true;
	}

	return false;
}