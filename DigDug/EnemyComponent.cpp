#include "EnemyComponent.h"

#include "GameObject.h"
#include "GridComponent.h"

#include "Utils.h"
#include "RoamingState.h"
#include "GhostState.h"
#include "ChaseState.h"
#include "CaughtState.h"
#include "EnemySquashedState.h"
#include "AnimationComponent.h"

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
}

void EnemyComponent::Update(float deltaTime)
{
	// Init states if necessary
	if (m_InitializedStates == false)
	{
		m_InitializedStates = true;
		InitStates();
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
	}
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

bool EnemyComponent::IsInsideEnemy(const glm::vec3 position) const
{
	return utils::IsInsideRect(position, m_pAnimationComponent->GetBoundingRect());
}
void EnemyComponent::SetCaught(bool isCaught)
{
	m_IsCaught = isCaught;

	// If caught, set state to caught
	if (isCaught)
	{
		m_pCurrentState->OnLeave(this);

		m_pCurrentState = m_pEnemyStates[static_cast<int>(enemy::Caught)].get();
		m_pCurrentState->OnEnter(this);
	}
}

void EnemyComponent::SetSquashed()
{
	m_pCurrentState->OnLeave(this);

	m_pCurrentState = m_pEnemyStates[static_cast<int>(enemy::Squashed)].get();
	m_pCurrentState->OnEnter(this);
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
void EnemyComponent::HandleEvent(unsigned int /*eventID*/)
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

	// Set default state
	m_pCurrentState = m_pEnemyStates[static_cast<int>(enemy::Roaming)].get();
	m_pCurrentState->OnEnter(this);
}