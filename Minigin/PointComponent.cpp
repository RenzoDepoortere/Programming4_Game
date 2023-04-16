#include "PointComponent.h"
#include "GameObject.h"

dae::PointComponent::PointComponent(dae::GameObject* pParentObject)
	: Component{ pParentObject }
{
	// Event
	m_BaseEvent.eventName = "PointEvent";
	m_BaseEvent.pGameObject = pParentObject;
	m_BaseEvent.extraArguments.resize(1);
}

void dae::PointComponent::SetCurrentPoint(int amount)
{
	// Set points
	m_CurrentPoints = amount;
	m_BaseEvent.extraArguments[0] = m_CurrentPoints;

	// Notify observers
	GetGameObject()->NotifyObservers(m_BaseEvent);
}