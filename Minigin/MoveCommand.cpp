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
	// ---------
	const grid::Cell currentCell{ m_pGrid->GetCell(startActorPos) };

	glm::vec3 desiredCellPos{};
	desiredCellPos.x = currentCell.centerPosition.x + m_MovementDirection.x * currentCell.size.x;
	desiredCellPos.y = currentCell.centerPosition.y + m_MovementDirection.y * currentCell.size.y;
	const grid::Cell desiredCell{ m_pGrid->GetCell(desiredCellPos) };

	bool insideX{};
	bool insideY{};
	const float buffer{ 2.5f };

	// If current cell is border cell
	// ------------------------------
	const bool isLeftBorderX{ currentCell.rowCol.y == 0 };
	const bool isRightBorderX{ currentCell.rowCol.y == m_pGrid->GetNrCols() - 1 };
	const bool isTopBorderY{ currentCell.rowCol.x == 0 };
	const bool isBotBorderY{ currentCell.rowCol.x == m_pGrid->GetNrRows() - 1 };

	if (isLeftBorderX)
	{
		// If currentPos is too close to left
		if (startActorPos.x < currentCell.centerPosition.x)
		{
			// And wants to go left, return
			if (m_MovementDirection.x < 0) return;
		}
	}
	if (isRightBorderX)
	{
		// If currentPos is too close to right
		if (currentCell.centerPosition.x < startActorPos.x)
		{
			// And wants to go right, return
			if (0 < m_MovementDirection.x) return;
		}
	}

	if (isTopBorderY)
	{
		// If currentPos is too close to top
		if (startActorPos.y < currentCell.centerPosition.y)
		{
			// And wants to go top, return
			if (m_MovementDirection.y < 0) return;
		}
	}
	if (isBotBorderY)
	{
		// If currentPos is too close to bot
		if (currentCell.centerPosition.y < startActorPos.y)
		{
			// And wants to go bot, return
			if (0 < m_MovementDirection.y) return;
		}
	}

	// Check if is free to move
	// ------------------------
	const bool moveXDirection{ abs(m_MovementDirection.x) > abs(m_MovementDirection.y) };

	insideX = desiredCell.centerPosition.x - buffer <= startActorPos.x && startActorPos.x <= desiredCell.centerPosition.x + buffer;
	insideY = desiredCell.centerPosition.y - buffer <= startActorPos.y && startActorPos.y <= desiredCell.centerPosition.y + buffer;

	const bool desiredCellInvalid{ desiredCell == grid::Cell{} };

	// If moving in x-direction
	if (moveXDirection)
	{
		// Not in the same y-range and desiredCell is not invalid
		if (insideY == false && desiredCellInvalid == false)
		{
			// Adjust direction
			if (startActorPos.y < desiredCell.centerPosition.y) desiredDirection.y = 1;
			else												desiredDirection.y = -1;
		}
		else 													desiredDirection = m_MovementDirection;
	}
	// Else if moving in y-direction
	else
	{
		// Not in the same x-range and desiredCell is not invalid
		if (insideX == false && desiredCellInvalid == false)
		{
			// Adjust
			if (startActorPos.x < desiredCell.centerPosition.x) desiredDirection.x = 1;
			else												desiredDirection.x = -1;
		}
		else													desiredDirection = m_MovementDirection;
	}
}