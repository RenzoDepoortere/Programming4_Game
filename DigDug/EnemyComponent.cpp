#include "EnemyComponent.h"

#include "GameObject.h"
#include "GridComponent.h"

#include "Utils.h"
#include "RoamingState.h"
#include "GhostState.h"
#include "ChaseState.h"
#include "CaughtState.h"
#include "EnemySquashedState.h"
#include "FleeState.h"
#include "AnimationComponent.h"
#include "CharacterComponent.h"

#include "DigDugSceneManager.h"

#include "ServiceLocator.h"
#include "EventManager.h"
#include "EventsEnum.h"
#include "InputManager.h"
#include "Renderer.h"

EnemyComponent::EnemyComponent(dae::GameObject* pParentObject)
	: Component{ pParentObject }
{
	// Get SFX ID
	const std::string fileName{ "Sound/Characters/Enemies/Squashed.wav" };
	dae::ServiceLocator::GetSoundSystem().SetID(event::RockBreak, fileName);

	// Set inactive function
	GetGameObject()->SetInActiveFunction(std::bind(&EnemyComponent::OnInactive, this));

	// Init state
	InitStates();
}

void EnemyComponent::Reset()
{
	dae::GameObject* pGameObject{ GetGameObject() };

	// Set default state
	// -----------------

	// Reset variables
	m_IsCaught = false;
	m_Blown = false;
	m_IsFleeing = false;
	m_IsDead = false;

	// Set state
	m_pCurrentState->OnLeave(this);
	m_pCurrentState = m_pEnemyStates[static_cast<int>(enemy::Roaming)].get();
	m_pCurrentState->OnEnter(this);

	m_CurrentStateID = enemy::Roaming;

	// Set parent
	// ----------
	if (pGameObject->GetParent() != m_pParent)
	{
		pGameObject->SetParent(m_pParent, false);
	}

	// Set active
	// ----------
	pGameObject->SetIsActive(true);
	pGameObject->SetIsHidden(false);
}

void EnemyComponent::Update(float deltaTime)
{
	UpdateStates(deltaTime);
	CheckPlayer();
}
void EnemyComponent::Render() const
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

void EnemyComponent::SetCaught(bool isCaught)
{
	m_IsCaught = isCaught;

	// If caught, set state to caught
	if (isCaught)
	{
		m_pCurrentState->OnLeave(this);

		m_pCurrentState = m_pEnemyStates[static_cast<int>(enemy::Caught)].get();
		m_CurrentStateID = enemy::Caught;
		m_pCurrentState->OnEnter(this);
	}
}
void EnemyComponent::SetSquashed()
{
	m_pCurrentState->OnLeave(this);

	m_pCurrentState = m_pEnemyStates[static_cast<int>(enemy::Squashed)].get();
	m_CurrentStateID = enemy::Squashed;
	m_pCurrentState->OnEnter(this);
}
void EnemyComponent::SetFlee()
{
	// Set flee to true
	m_IsFleeing = true;

	// If currently in squash state or ghost state
	if (dynamic_cast<enemy::EnemySquashedState*>(m_pCurrentState) || dynamic_cast<enemy::GhostState*>(m_pCurrentState))
	{
		// Do nothing
		return;
	}

	// Else, just swap to flee state
	m_pCurrentState->OnLeave(this);

	m_pCurrentState = m_pEnemyStates[static_cast<int>(enemy::Flee)].get();
	m_CurrentStateID = enemy::Flee;
	m_pCurrentState->OnEnter(this);
}

dae::AnimationComponent* EnemyComponent::GetAnimationComponent()
{
	// Return component if available
	if (m_pAnimationComponent) return m_pAnimationComponent;

	// Else, get component
	m_pAnimationComponent = GetGameObject()->GetComponent<dae::AnimationComponent>();
	return m_pAnimationComponent;
}

void EnemyComponent::SetControl(unsigned long controllerID)
{
	// No controller
	if (controllerID == -1)
	{
		m_IsControlled = false;
	}
	// Else, store ID
	else
	{
		m_IsControlled = true;
		m_ControllerID = controllerID;
	}
}

// Only gets called when got hit by rock 
void EnemyComponent::HandleEvent(unsigned int /*eventID*/, void* /*extraInfo*/)
{
	// Play squashed SFX
	const int volume{ 100 };
	const int loops{ 0 };

	dae::ServiceLocator::GetSoundSystem().PlayAudio(event::RockBreak, volume, loops);
}
void EnemyComponent::OnSubjectDestroy()
{
}

void EnemyComponent::InitStates()
{
	// Create states
	m_pEnemyStates[static_cast<int>(enemy::Roaming)] = std::make_unique<enemy::RoamingState>();
	m_pEnemyStates[static_cast<int>(enemy::Ghost)] = std::make_unique<enemy::GhostState>();
	m_pEnemyStates[static_cast<int>(enemy::Chase)] = std::make_unique<enemy::ChaseState>();
	m_pEnemyStates[static_cast<int>(enemy::Caught)] = std::make_unique<enemy::CaughtState>();
	m_pEnemyStates[static_cast<int>(enemy::Squashed)] = std::make_unique<enemy::EnemySquashedState>();
	m_pEnemyStates[static_cast<int>(enemy::Flee)] = std::make_unique<enemy::FleeState>();

	// Set default state
	m_pCurrentState = m_pEnemyStates[static_cast<int>(enemy::Roaming)].get();
	m_CurrentStateID = enemy::Roaming;
}

void EnemyComponent::OnInactive()
{
	// Return if is changing level
	if (digdug::DigDugSceneManager::GetInstance().GetIsChangingLevel()) return;

	const bool isBeingKilled{ m_CurrentStateID == enemy::Squashed || m_CurrentStateID == enemy::Caught };
	if (isBeingKilled == false) return;

	if (m_IsDead) return;
	m_IsDead = true;

	// Send event
	// ----------

	// Check if squashed
	bool isInSquashedState{ false };
	if (m_CurrentStateID == enemy::Squashed) isInSquashedState = true;

	// Get cell
	auto pGrid{ digdug::DigDugSceneManager::GetInstance().GetGrid() };
	const glm::vec3 currentPos{ GetGameObject()->GetWorldPosition() };
	grid::Cell* pCurrentCell{ pGrid->GetCell(currentPos) };
	
	// Send event
	dae::EventManager<grid::Cell*, void*, void*>::GetInstance().SendEvent(event::EnemyDeath, pCurrentCell, &m_EnemyBehavior, &isInSquashedState);
}

void EnemyComponent::UpdateStates(float deltaTime)
{
	// On first update, call onEnter
	if (m_StateInitialized == false)
	{
		m_StateInitialized = true;
		m_pCurrentState->OnEnter(this);
	}

	// Update currentState
	enemy::EnemyStates state{};
	state = m_pCurrentState->Update(this, deltaTime);

	// Change state if asked
	if (state != enemy::NR_STATES)
	{
		m_pCurrentState->OnLeave(this);

		m_pCurrentState = m_pEnemyStates[static_cast<int>(state)].get();
		m_pCurrentState->OnEnter(this);

		m_CurrentStateID = state;
	}
}
void EnemyComponent::CheckPlayer()
{
	// If in Caught state, return
	if (m_CurrentStateID == enemy::Caught) return;

	const utils::Rect ownBoundingRect{ m_pAnimationComponent->GetBoundingRect() };
	utils::Rect boundingRect{};

	for (const auto& currentPlayer : digdug::DigDugSceneManager::GetInstance().GetCharacters())
	{
		// If in hitState, continue
		if (currentPlayer->GetCurrentStateID() == player::Hit) continue;

		// Check if is not dead
		if (currentPlayer->GetGameObject()->GetIsActive() == false) continue;

		// Check if overlaps
		boundingRect = currentPlayer->GetAnimationComponent()->GetBoundingRect();
		if (utils::RectOverlaps(ownBoundingRect, boundingRect))
		{
			// Set to hit
			currentPlayer->SetHit();
		}
	}
}