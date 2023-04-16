#include "PointsDisplay.h"
#include "TextComponent.h"
#include "PointComponent.h"
#include "GameObject.h"

#include <cassert>

PointsDisplay::PointsDisplay(dae::GameObject* pGameObjectToFollow, dae::TextComponent* pTextComponent)
	: Observer{ pGameObjectToFollow }
	, m_pTextComponent{ pTextComponent }
{
	// Point component
	m_pPointComponent = GetGameObject()->GetComponent<dae::PointComponent>();

	// Set text
	m_BaseString = "Points: ";
	m_pTextComponent->SetText(m_BaseString + std::to_string(m_pPointComponent->GetCurrentPoint()));

	// Event
	m_EventName = "PointEvent";
}

void PointsDisplay::Notify(const Event& event)
{
	if (event.eventName == m_EventName)
	{
		// Get points
		const int newPoints{ m_pPointComponent->GetCurrentPoint() };

		// Set text
		m_pTextComponent->SetText(m_BaseString + std::to_string(newPoints));
	}
}