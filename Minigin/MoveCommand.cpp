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

		//// Calculate Direction
		//const float buffer{ 2.5f };
		//const bool insideX{ desiredCell.centerPosition.x - buffer <= startActorPos.x && startActorPos.x <= desiredCell.centerPosition.x + buffer };
		//const bool insideY{ desiredCell.centerPosition.y - buffer <= startActorPos.y && startActorPos.y <= desiredCell.centerPosition.y + buffer };

		//glm::vec2 desiredDirection{ m_MovementDirection };
		//if (insideX == false)
		//{
		//	desiredDirection.x = (desiredCell.centerPosition.x - currentCell.centerPosition.x) / currentCell.size.x;
		//	desiredDirection.y = 0;

		//	// Calculate movement
		//	actorPos.x += (m_MovementSpeed * desiredDirection.x) * deltaTime;
		//	actorPos.y += (m_MovementSpeed * desiredDirection.y) * deltaTime;

		//	// Set pos
		//	pActor->SetWorldPosition(actorPos);

		//	return;
		//}
		//if (insideY == false)
		//{
		//	desiredDirection.x = 0;
		//	desiredDirection.y = (desiredCell.centerPosition.y - currentCell.centerPosition.y) / currentCell.size.y;

		//	// Calculate movement
		//	actorPos.x += (m_MovementSpeed * desiredDirection.x) * deltaTime;
		//	actorPos.y += (m_MovementSpeed * desiredDirection.y) * deltaTime;

		//	// Set pos
		//	pActor->SetWorldPosition(actorPos);

		//	return;
		//}
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