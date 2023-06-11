#include "SelectionComponent.h"

#include "GameObject.h"

#include "DigDugSceneManager.h"

#include "InputManager.h"
#include "EventManager.h"
#include "EventsEnum.h"

SelectionComponent::SelectionComponent(dae::GameObject* pParentObject)
	: Component{ pParentObject }
	, m_XPos{ 180 }
{
	// Subscribe to event
	dae::EventManager<float>::GetInstance().Subscribe(event::StartMenu, this);
}
SelectionComponent::~SelectionComponent()
{
	// Unsubscribe to events
	if (dae::EventManager<float>::GetIsDestroyed() == false)
	{
		dae::EventManager<float>::GetInstance().Unsubscribe(event::StartMenu, this);
	}
}

void SelectionComponent::HandleEvent(unsigned int /*eventID*/, float /*deltaTime*/)
{
	// Check if is active
	if (GetGameObject()->GetIsActive() == false) return;

	// Check where arrow is

	// Execute that thing
	digdug::DigDugSceneManager::GetInstance().NextLevel();
}
void SelectionComponent::OnSubjectDestroy()
{
}