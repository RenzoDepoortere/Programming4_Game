#include "MoveCommand.h"
#include "GameObject.h"
#include <iostream>

// Move
// ****
dae::MoveCommand::MoveCommand(GameObject* pActor, glm::vec2 movementDirection, float movementSpeed)
	: GameObjectCommand{ pActor }
	, m_MovementDirection{ movementDirection }
	, m_MovementSpeed{ movementSpeed }
{
}

void dae::MoveCommand::Execute(float deltaTime)
{
	// Get actor
	GameObject* pActor{ GetGameObject() };
	glm::vec3 actorPos{ pActor->GetWorldPosition() };

	// Calculate movement
	actorPos.x += (m_MovementSpeed * m_MovementDirection.x) * deltaTime;
	actorPos.y += (m_MovementSpeed * m_MovementDirection.y) * deltaTime;

	// Set pos
	pActor->SetWorldPosition(actorPos);
}