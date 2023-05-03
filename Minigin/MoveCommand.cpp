#include "MoveCommand.h"
#include "GameObject.h"
#include "../DigDug/GridComponent.h"
#include <iostream>

// Move
// ****
dae::MoveCommand::MoveCommand(GameObject* pActor, glm::vec2 movementDirection, float movementSpeed, grid::GridComponent* pGrid)
	: GameObjectCommand{ pActor }
	, m_MovementDirection{ movementDirection }
	, m_MovementSpeed{ movementSpeed }
	, m_pGrid{ pGrid }
{
}

void dae::MoveCommand::Execute(float deltaTime)
{
	// Get actor
	GameObject* pActor{ GetGameObject() };
	const glm::vec3 startActorPos{ pActor->GetWorldPosition() };
	glm::vec3 actorPos{ startActorPos };

	// Calculate movement
	actorPos.x += (m_MovementSpeed * m_MovementDirection.x) * deltaTime;
	actorPos.y += (m_MovementSpeed * m_MovementDirection.y) * deltaTime;

	// If is on grid
	if (m_pGrid)
	{
		// Get cells
		const grid::Cell currentCell{ m_pGrid->GetCell(startActorPos) };
		const grid::Cell desiredCell{ m_pGrid->GetCell(actorPos) };

		// If desired cell equals the default, then is an invalid cell and return
		if (desiredCell == grid::Cell{}) return;

		const float buffer{ 2.5f };
		// If on same x-axis
		if (desiredCell.worldPosition.x - buffer <= currentCell.worldPosition.x && desiredCell.worldPosition.x <= desiredCell.worldPosition.x + buffer)
		{
			// Disable y-movement
			actorPos.y = startActorPos.y;
		}
		// Else, if on same y-axis
		else if (desiredCell.worldPosition.y - buffer <= currentCell.worldPosition.y && desiredCell.worldPosition.y <= desiredCell.worldPosition.y + buffer)
		{
			// Disable x-movement
			actorPos.x = startActorPos.x;
		}
		// Else
		else
		{
			// Check which axis is more dominant
			//const bool yMoreDominant{ m_MovementDirection.x < m_MovementDirection.y };
		}
	
	}

	// Set pos
	pActor->SetWorldPosition(actorPos);
}