#include "EnemyComponent.h"

#include "GameObject.h"
#include "GridComponent.h"

#include "Utils.h"
#include "EnemyState.h"
#include "RoamingState.h"

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
	m_pCurrentState->Update(this, deltaTime);
}

void EnemyComponent::InitStates()
{
	// Create states
	m_pEnemyStates[static_cast<int>(Enemy::Roaming)] = std::make_unique<Enemy::RoamingState>();

	// Set default state
	m_pCurrentState = m_pEnemyStates[static_cast<int>(Enemy::Roaming)].get();
	m_pCurrentState->OnEnter(this);
}