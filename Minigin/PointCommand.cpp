#include "PointCommand.h"
#include "PointComponent.h"
#include "GameObject.h"

dae::PointCommand::PointCommand(GameObject* pActor)
	: GameObjectCommand{ pActor }
{
	// Get pointComponent
	m_pPointComponent = pActor->GetComponent<dae::PointComponent>();
}

void dae::PointCommand::Execute(float)
{
	// Add points
	const int pointAmount{ 5 };
	m_pPointComponent->SetCurrentPoint(m_pPointComponent->GetCurrentPoint() + pointAmount);
}