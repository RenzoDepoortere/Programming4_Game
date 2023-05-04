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
	// Set in first cell
	if (pGrid)
	{
		const glm::vec2 firstCellPos{ pGrid->GetCell(0, 0, 0).centerPosition };
		pActor->SetWorldPosition(firstCellPos.x, firstCellPos.y, 0.f);
	}
}

void dae::MoveCommand::Execute(float deltaTime)
{
	// Get actor
	GameObject* pActor{ GetGameObject() };
	const glm::vec3 startActorPos{ pActor->GetWorldPosition() };
	glm::vec3 actorPos{ startActorPos };
	
	// Calculate direction
	glm::vec2 desiredDirection{ m_MovementDirection };
	if (m_pGrid) GridMovement(desiredDirection, startActorPos);

	// Calculate movement
	actorPos.x += (m_MovementSpeed * desiredDirection.x) * deltaTime;
	actorPos.y += (m_MovementSpeed * desiredDirection.y) * deltaTime;

	// Set pos
	pActor->SetWorldPosition(actorPos);
}

void dae::MoveCommand::GridMovement(glm::vec2& desiredDirection, const glm::vec3& startActorPos)
{
	// Reset direction
	desiredDirection = {};

	// Get cells
	const grid::Cell currentCell{ m_pGrid->GetCell(startActorPos) };

	glm::vec3 desiredCellPos{};
	desiredCellPos.x = startActorPos.x + m_MovementDirection.x * currentCell.size.x;
	desiredCellPos.y = startActorPos.y + m_MovementDirection.y * currentCell.size.y;
	const grid::Cell desiredCell{ m_pGrid->GetCell(desiredCellPos) };

	// If desired cell is invalid, return
	if (desiredCell == grid::Cell{}) return;

	// Check if is free to move
	const float buffer{ 2.5f };
	const bool insideX{ desiredCell.centerPosition.x - buffer <= startActorPos.x && startActorPos.x <= desiredCell.centerPosition.x + buffer };
	const bool insideY{ desiredCell.centerPosition.y - buffer <= startActorPos.y && startActorPos.y <= desiredCell.centerPosition.y + buffer };

	// If moving in x-direction
	const bool moveXDirection{ abs(m_MovementDirection.x) > abs(m_MovementDirection.y) };
	if (moveXDirection)
	{
		// And not in the same y-range
		if (insideY == false)
		{
			// Adjust direction
			if (currentCell.rowCol.y < desiredCell.rowCol.y) desiredDirection.y = 1;
			else											 desiredDirection.y = -1;
		}
		else 												 desiredDirection = m_MovementDirection;
	}
	// Else if moving in y-direction
	else
	{
		// And not in the same x-range
		if (insideX == false)
		{
			// Adjust
			if (currentCell.rowCol.x < desiredCell.rowCol.x) desiredDirection.x = 1;
			else											 desiredDirection.x = -1;
		}
		else												 desiredDirection = m_MovementDirection;
	}
}