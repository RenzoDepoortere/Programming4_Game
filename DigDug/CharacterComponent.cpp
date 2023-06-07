#include "CharacterComponent.h"

#include "AnimationComponent.h"
#include "GameObject.h"
#include "DiggingState.h"
#include "ShootingState.h"
#include "BlowingState.h"

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
	player::PlayerStates state{};
	state = m_pCurrentState->Update(this, deltaTime);

	// Change state if asked
	if (state != player::NR_STATES)
	{
		m_pCurrentState->OnLeave(this);

		m_pCurrentState = m_pPlayerStates[static_cast<int>(state)].get();
		m_pCurrentState->OnEnter(this);
	}
}

void CharacterComponent::SetLookingDirection(player::LookingDirection lookingDirection)
{
	// Rotate accordingly
	auto pObject{ GetGameObject() };
	switch (lookingDirection)
	{
	case player::Up:
		pObject->SetRotation(90.f);
		m_pAnimationComponent->SetFlip(true);
		break;

	case player::Down:
		pObject->SetRotation(90.f);
		m_pAnimationComponent->SetFlip(false);
		break;

	case player::Left:
		pObject->SetRotation(0.f);
		m_pAnimationComponent->SetFlip(true);
		break;

	case player::Right:
		pObject->SetRotation(0.f);
		m_pAnimationComponent->SetFlip(false);
		break;
	}

	// Set lookingDirection
	m_CurrentLookingDirection = lookingDirection;
}

void CharacterComponent::InitStates()
{
	// Create states
	m_pPlayerStates[static_cast<int>(player::Digging)] = std::make_unique<player::DiggingState>();
	m_pPlayerStates[static_cast<int>(player::Shooting)] = std::make_unique<player::ShootingState>();
	m_pPlayerStates[static_cast<int>(player::Blowing)] = std::make_unique<player::BlowingState>();

	// Set default state
	m_pCurrentState = m_pPlayerStates[static_cast<int>(player::Digging)].get();
	m_pCurrentState->OnEnter(this);
}