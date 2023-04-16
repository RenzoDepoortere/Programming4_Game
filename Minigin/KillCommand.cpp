#include "KillCommand.h"
#include "GameObject.h"
#include "HealthComponent.h"

dae::KillCommand::KillCommand(GameObject* pActor)
	: GameObjectCommand{ pActor }
{
	// Get healthComponent
	m_pHealthComponent = pActor->GetComponent<dae::HealthComponent>();
}

void dae::KillCommand::Execute(float)
{
	// Set health to 0 --> kill player
	m_pHealthComponent->SetCurrentHealth(0.f);
}