#include "EnemyComponent.h"

#include "GameObject.h"
#include "GridComponent.h"

#include "Utils.h"
#include "RoamingState.h"
#include "GhostState.h"
#include "ChaseState.h"
#include "CaughtState.h"
#include "AnimationComponent.h"

#include "InputManager.h"

EnemyComponent::EnemyComponent(dae::GameObject* pParentObject)
	: Component{ pParentObject }
{
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

void EnemyComponent::InitStates()
{
	// Create states
	m_pEnemyStates[static_cast<int>(enemy::Roaming)] = std::make_unique<enemy::RoamingState>();
	m_pEnemyStates[static_cast<int>(enemy::Ghost)] = std::make_unique<enemy::GhostState>();
	m_pEnemyStates[static_cast<int>(enemy::Chase)] = std::make_unique<enemy::ChaseState>();
	m_pEnemyStates[static_cast<int>(enemy::Caught)] = std::make_unique<enemy::CaughtState>();

	// Set default state
	m_pCurrentState = m_pEnemyStates[static_cast<int>(enemy::Roaming)].get();
	m_pCurrentState->OnEnter(this);
}