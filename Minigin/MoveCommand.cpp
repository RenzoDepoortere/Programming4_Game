#include "MoveCommand.h"
#include "GameObject.h"
#include "../DigDug/GridComponent.h"

#include <iostream>
#include <math.h>

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

	// If is on grid
	if (m_pGrid)
	{
		// Get cells
		const grid::Cell currentCell{ m_pGrid->GetCell(startActorPos) };

		glm::vec3 desiredCellPos{};
		desiredCellPos.x = startActorPos.x + m_MovementDirection.x * currentCell.size.x;
		desiredCellPos.y = startActorPos.y + m_MovementDirection.y * currentCell.size.y;

		const grid::Cell desiredCell{ m_pGrid->GetCell(desiredCellPos) };

		// If desired cell is invalid, return
		if (desiredCell == grid::Cell{}) return;

		// Set pos
		actorPos = glm::vec3{ desiredCell.centerPosition, 0.f };
		pActor->SetWorldPosition(actorPos);
	}
	else
	{
		// Calculate movement
		actorPos.x += (m_MovementSpeed * m_MovementDirection.x) * deltaTime;
		actorPos.y += (m_MovementSpeed * m_MovementDirection.y) * deltaTime;

		// Set pos
		pActor->SetWorldPosition(actorPos);
	}
}