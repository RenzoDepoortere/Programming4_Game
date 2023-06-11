#include "SelectionComponent.h"

#include "GameObject.h"

#include "DigDugSceneManager.h"

#include "InputManager.h"
#include "EventManager.h"
#include "EventsEnum.h"

SelectionComponent::SelectionComponent(dae::GameObject* pParentObject)
	: Component{ pParentObject }
	, m_MaxCooldown{ 0.25f }
	, m_StartCooldown{ 0.5f }
{
}
SelectionComponent::~SelectionComponent()
{
	// Unsubscribe to events
	if (dae::EventManager<float>::GetIsDestroyed() == false)
	{
		dae::EventManager<float>::GetInstance().Unsubscribe(event::StartMenu, this);

		dae::EventManager<float>::GetInstance().Unsubscribe(event::KeyboardUp, this);
		dae::EventManager<float>::GetInstance().Unsubscribe(event::KeyboardDown, this);

		dae::EventManager<float>::GetInstance().Unsubscribe(event::ControllerUp_2, this);
		dae::EventManager<float>::GetInstance().Unsubscribe(event::ControllerDown_2, this);

		dae::EventManager<float>::GetInstance().Unsubscribe(event::ControllerUp_1, this);
		dae::EventManager<float>::GetInstance().Unsubscribe(event::ControllerDown_1, this);
	}
}

void SelectionComponent::Update(float deltaTime)
{
	if (0.f < m_Cooldown)
	{
		m_Cooldown -= deltaTime;
	}
	if (0.f < m_StartCooldown)
	{
		m_StartCooldown -= deltaTime;
	}

	if (m_HasToSubscribe)
	{
		m_HasToSubscribe = false;
		Subscribe();
	}
}

void SelectionComponent::HandleEvent(unsigned int eventID, float /*deltaTime*/)
{
	// Check if is active
	if (GetGameObject()->GetIsActive() == false) return;

	// Check eventID
	switch (eventID)
	{
	case event::StartMenu:
		Activate();
		break;

	case event::KeyboardUp:
	case event::ControllerUp_1:
	case event::ControllerUp_2:
		GoUp();
		break;

	case event::KeyboardDown:
	case event::ControllerDown_1:
	case event::ControllerDown_2:
		GoDown();
		break;
	}
}
void SelectionComponent::OnSubjectDestroy()
{
}

void SelectionComponent::Activate()
{
	// Reset cooldown
	if (0.f < m_StartCooldown) return;
	m_StartCooldown = 0.5f;

	// Execute function of position
	m_Functions[m_CurrentPos]();
}
void SelectionComponent::Subscribe()
{
	// Subscribe to event
	dae::EventManager<float>::GetInstance().Subscribe(event::StartMenu, this);

	dae::EventManager<float>::GetInstance().Subscribe(event::KeyboardUp, this);
	dae::EventManager<float>::GetInstance().Subscribe(event::KeyboardDown, this);

	dae::EventManager<float>::GetInstance().Subscribe(event::ControllerUp_2, this);
	dae::EventManager<float>::GetInstance().Subscribe(event::ControllerDown_2, this);

	dae::EventManager<float>::GetInstance().Subscribe(event::ControllerUp_1, this);
	dae::EventManager<float>::GetInstance().Subscribe(event::ControllerDown_1, this);
}

void SelectionComponent::GoDown()
{
	// return if at end
	if (m_CurrentPos == m_Positions.size() - 1) return;

	// Return if on cooldown
	if (0.f < m_Cooldown) return;
	m_Cooldown = m_MaxCooldown;

	// Go down one
	++m_CurrentPos;
	GetGameObject()->SetWorldPosition(m_XPos, m_Positions[m_CurrentPos], 0.f);
}
void SelectionComponent::GoUp()
{
	// return if at end
	if (m_CurrentPos == 0) return;

	// Return if on cooldown
	if (0.f < m_Cooldown) return;
	m_Cooldown = m_MaxCooldown;

	// Go up one
	--m_CurrentPos;
	GetGameObject()->SetWorldPosition(m_XPos, m_Positions[m_CurrentPos], 0.f);
}