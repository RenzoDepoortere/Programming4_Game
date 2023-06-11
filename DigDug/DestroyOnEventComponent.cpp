#include "DestroyOnEventComponent.h"

#include "GameObject.h"
#include "EventManager.h"

#include "DigDugSceneManager.h"

DestroyOnEventComponent::DestroyOnEventComponent(dae::GameObject* pParentObject)
	: Component{ pParentObject }
{
}
DestroyOnEventComponent::~DestroyOnEventComponent()
{
	// Unsubscribe
	if (dae::EventManager<float>::GetInstance().GetIsDestroyed() == false)
	{
		dae::EventManager<float>::GetInstance().Unsubscribe(m_EventID, this);
	}
}

void DestroyOnEventComponent::SetEvent(unsigned int eventID)
{
	m_EventID = eventID;

	// Subscribe to event
	dae::EventManager<float>::GetInstance().Subscribe(m_EventID, this);
}

void DestroyOnEventComponent::HandleEvent(unsigned int /*eventID*/, float /*deltaTime*/)
{
	// Show menu
	digdug::DigDugSceneManager::GetInstance().ShowMenu();

	// Remove
	GetGameObject()->RemoveObject();
}
void DestroyOnEventComponent::OnSubjectDestroy()
{
}