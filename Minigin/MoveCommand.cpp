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
		const glm::vec2 firstCellPos{ pGrid->GetCell(0, 0, 0)->centerPosition };
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

// Todo: clean up function, is too big
// and is probably done in a "dumb" way
void dae::MoveCommand::GridMovement(glm::vec2& desiredDirection, const glm::vec3& startActorPos)
{
	// Reset direction
	desiredDirection = {};

	// Get cells
	// ---------
	const grid::Cell* pCurrentCell{ m_pGrid->GetCell(startActorPos) };

	glm::vec3 desiredCellPos{};
	desiredCellPos.x = pCurrentCell->centerPosition.x + m_MovementDirection.x * pCurrentCell->size.x;
	desiredCellPos.y = pCurrentCell->centerPosition.y + m_MovementDirection.y * pCurrentCell->size.y;
	const grid::Cell* pDesiredCell{ m_pGrid->GetCell(desiredCellPos) };

	bool insideX{};
	bool insideY{};
	const float buffer{ 2.5f };

	// If current cell is border cell
	// ------------------------------
	const bool isLeftBorderX{ pCurrentCell->rowCol.y == 0 };
	const bool isRightBorderX{ pCurrentCell->rowCol.y == m_pGrid->GetNrCols() - 1 };
	const bool isTopBorderY{ pCurrentCell->rowCol.x == 0 };
	const bool isBotBorderY{ pCurrentCell->rowCol.x == m_pGrid->GetNrRows() - 1 };

	if (isLeftBorderX)
	{
		// If currentPos is too close to left
		if (startActorPos.x < pCurrentCell->centerPosition.x)
		{
			// And wants to go left, return
			if (m_MovementDirection.x < 0) return;
		}
	}
	if (isRightBorderX)
	{
		// If currentPos is too close to right
		if (pCurrentCell->centerPosition.x < startActorPos.x)
		{
			// And wants to go right, return
			if (0 < m_MovementDirection.x) return;
		}
	}

	if (isTopBorderY)
	{
		// If currentPos is too close to top
		if (startActorPos.y < pCurrentCell->centerPosition.y)
		{
			// And wants to go top, return
			if (m_MovementDirection.y < 0) return;
		}
	}
	if (isBotBorderY)
	{
		// If currentPos is too close to bot
		if (pCurrentCell->centerPosition.y < startActorPos.y)
		{
			// And wants to go bot, return
			if (0 < m_MovementDirection.y) return;
		}
	}

	if (pDesiredCell)
	{
		// If desired cell contains rock
		// -----------------------------
		if (pDesiredCell->containsRock)
		{
			const bool leftOfCell{ pCurrentCell->centerPosition.x < startActorPos.x&& startActorPos.x < pDesiredCell->centerPosition.x };
			const bool rightOfCell{ pDesiredCell->centerPosition.x < startActorPos.x&& startActorPos.x < pCurrentCell->centerPosition.x };
			const bool topOfCell{ pCurrentCell->centerPosition.y < startActorPos.y&& startActorPos.y < pDesiredCell->centerPosition.y };
			const bool botOfCell{ pDesiredCell->centerPosition.y < startActorPos.y&& startActorPos.y < pCurrentCell->centerPosition.y };

			if (leftOfCell && 0 < m_MovementDirection.x) return;
			if (rightOfCell && m_MovementDirection.x < 0) return;
			if (topOfCell && 0 < m_MovementDirection.y) return;
			if (botOfCell && m_MovementDirection.y < 0) return;
		}


		// Check if is free to move
		// ------------------------
		const bool moveXDirection{ abs(m_MovementDirection.x) > abs(m_MovementDirection.y) };

		insideX = pDesiredCell->centerPosition.x - buffer <= startActorPos.x && startActorPos.x <= pDesiredCell->centerPosition.x + buffer;
		insideY = pDesiredCell->centerPosition.y - buffer <= startActorPos.y && startActorPos.y <= pDesiredCell->centerPosition.y + buffer;

		// If moving in x-direction
		if (moveXDirection)
		{
			// Not in the same y-range and desiredCell is not invalid
			if (insideY == false)
			{
				// Adjust direction
				if (startActorPos.y < pDesiredCell->centerPosition.y) desiredDirection.y = 1;
				else												  desiredDirection.y = -1;
			}
			else													  desiredDirection = m_MovementDirection;
		}
		// Else if moving in y-direction
		else
		{
			// Not in the same x-range and desiredCell is not invalid
			if (insideX == false)
			{
				// Adjust
				if (startActorPos.x < pDesiredCell->centerPosition.x) desiredDirection.x = 1;
				else												  desiredDirection.x = -1;
			}
			else													  desiredDirection = m_MovementDirection;
		}
	}
	else
	{
		desiredDirection = m_MovementDirection;
	}
}