#include "LivesDisplay.h"
#include "TextComponent.h"
#include <iostream>

LivesDisplay::LivesDisplay(dae::GameObject* pGameObjectToFollow, dae::TextComponent* pTextComponent)
	: Observer{ pGameObjectToFollow }
	, m_pTextComponent{ pTextComponent }
{
	// Set lives
	m_MaxLives = 3;
	m_CurrentLives = m_MaxLives;

	// Set text
	m_BaseString = "Lives: ";
	m_pTextComponent->SetText(m_BaseString + std::to_string(m_CurrentLives));

	// Event
	m_EventName = "PlayerDied";
}

void LivesDisplay::Notify(const Event& event)
{
	if (event.eventName == m_EventName)
	{
		// Set lives
		--m_CurrentLives;

		// Set text
		m_pTextComponent->SetText(m_BaseString + std::to_string(m_CurrentLives));
	}
}