#include "CharacterComponent.h"

#include "GameObject.h"
#include "DiggingState.h"
#include "ShootingState.h"

CharacterComponent::CharacterComponent(dae::GameObject* pParentObject)
	: Component{ pParentObject }
{

}

void CharacterComponent::Update(float deltaTime)
{
	// Init states if necessary
	if (m_InitializedStates == false)
	{
		m_InitializedStates = true;
		InitStates();
	}

	// Update currentState
	Player::PlayerStates state{};
	state = m_pCurrentState->Update(this, deltaTime);

	// Change state if asked
	if (state != Player::NR_STATES)
	{
		m_pCurrentState->OnLeave(this);

		m_pCurrentState = m_pPlayerStates[static_cast<int>(state)].get();
		m_pCurrentState->OnEnter(this);
	}
}

void CharacterComponent::InitStates()
{
	// Create states
	m_pPlayerStates[static_cast<int>(Player::Digging)] = std::make_unique<Player::DiggingState>();
	m_pPlayerStates[static_cast<int>(Player::Shooting)] = std::make_unique<Player::ShootingState>();

	// Set default state
	m_pCurrentState = m_pPlayerStates[static_cast<int>(Player::Digging)].get();
	m_pCurrentState->OnEnter(this);
}