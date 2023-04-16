#include "HealthComponent.h"
#include "GameObject.h"

dae::HealthComponent::HealthComponent(dae::GameObject* pParentObject)
	: Component{ pParentObject }
{
	// Defaults
	m_MaxHealth = 100.f;
	m_CurrentHealth = m_MaxHealth;

	m_StartPos = pParentObject->GetWorldPosition();

	// Event
	m_BaseEvent.eventName = "PlayerDied";
	m_BaseEvent.pGameObject = pParentObject;

}

void dae::HealthComponent::SetCurrentHealth(float amount)
{
	// Set health
	m_CurrentHealth = amount;

	// Check if dead
	if (m_CurrentHealth <= 0.f)
	{
		// Notify observers
		GetGameObject()->NotifyObservers(m_BaseEvent);

		// Respawn
		Respawn();
	}
}

void dae::HealthComponent::Respawn()
{
	// Set back to spawnPoint
	GetGameObject()->SetWorldPosition(m_StartPos);

	// Set currentHealth to max
	m_CurrentHealth = m_MaxHealth;
}