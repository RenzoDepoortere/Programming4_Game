#include "ChaseState.h"

#include "EnemyComponent.h"
#include "GridComponent.h"
#include "GridHelpers.h"
#include "CharacterComponent.h"
#include "GameObject.h"

void enemy::ChaseState::OnEnter(EnemyComponent* pEnemy)
{
	// Prepare variables
	// -----------------
	glm::vec3 characterPos{};
	CharacterComponent* pClosestCharacter{ nullptr };
	float closestDistance{ FLT_MAX };
	float currentDistance{};

	dae::GameObject* pEnemyGameObject{ pEnemy->GetGameObject() };
	const glm::vec3 currentPos{ pEnemyGameObject->GetWorldPosition() };

	// Check for closest character
	// ---------------------------
	for (const auto& currentCharacter : pEnemy->GetCharacters())
	{
		// Get cell
		characterPos = currentCharacter->GetGameObject()->GetWorldPosition();

		// Get distance
		currentDistance = utils::GetSqrdMagnitude(characterPos - currentPos);

		// Check if closer
		if (currentDistance < closestDistance)
		{
			closestDistance = currentDistance;
			pClosestCharacter = currentCharacter;
		}
	}

	// Store closest one
	m_pCharacterToChase = pClosestCharacter;

	// Create moveCommand
	// ------------------
	const enemy::BehaviorData behaviorData{ pEnemy->GetBehaviorData() };
	grid::GridComponent* pGrid{ pEnemy->GetGrid() };

	m_pMoveCommand = std::make_unique<dae::MoveCommand>(pEnemyGameObject, glm::vec2{ 0, 1 }, behaviorData.movementSpeed, pGrid, false);

	// Calculate path
	// --------------
	m_DesiredPath = CalculatePath(pEnemy);

	m_CheckInterval = behaviorData.detectionInterval;
	m_CurrentTime = m_CheckInterval;
}
void enemy::ChaseState::OnLeave(EnemyComponent* /*pEnemy*/)
{

}

enemy::EnemyStates enemy::ChaseState::Update(EnemyComponent* pEnemy, float deltaTime)
{
	// Countdown
	m_CurrentTime -= deltaTime;
	if (m_CurrentTime <= 0.f)
	{
		m_CurrentTime = m_CheckInterval;

		// Check if should still chase

		// Calculate path
		m_DesiredPath = CalculatePath(pEnemy);
	}

	// Follow path
	FollowPath(pEnemy, deltaTime);

	// Return
	return NR_STATES;
}

std::deque<grid::Cell*> enemy::ChaseState::CalculatePath(EnemyComponent* pEnemy)
{
	// Get cells
	// ---------
	grid::GridComponent* pGrid{ pEnemy->GetGrid() };

	// Own
	const glm::vec3 currentPos{ pEnemy->GetGameObject()->GetWorldPosition() };
	grid::Cell* pStartCell{ pGrid->GetCell(currentPos) };

	// Player
	const glm::vec3 playerPos{ m_pCharacterToChase->GetGameObject()->GetWorldPosition() };
	grid::Cell* pPlayerCell{ pGrid->GetCell(playerPos) };

	// StartVariables
	// --------------
	std::deque<grid::Cell*> path;
	std::vector<CellRecord> openList;
	std::vector<CellRecord> closedList;
	CellRecord currentRecord;

	// StartRecord
	CellRecord startRecord;
	startRecord.pCell = pStartCell;
	startRecord.pPreviousCell = nullptr;
	startRecord.costSoFar = 0;
	startRecord.estimatedTotalCost = GetHeuristicCost(pStartCell, pPlayerCell);

	openList.push_back(startRecord);

	bool goToNextConnection{ false };
	CellRecord existingRecord;
	existingRecord.pCell = nullptr;

	bool hasSameNode{};
	bool hasSameConnection{};

	// Start the loop
	// --------------
	while (openList.empty() == false)
	{
		// Get lowestCost node from openList
		currentRecord = *std::min(openList.begin(), openList.end());

		// Check if lowestCost node is endNode
		if (currentRecord.pCell == pPlayerCell)
		{
			break;
		}

		// Get all connections
		for (auto& pConnectedCell : currentRecord.pCell->pConnectedCells)
		{
			//---------------//
			//--First Check--//
			//---------------//

			// Check if node is already on closedList
			for (auto& nodeRecord : closedList)
			{
				if (pConnectedCell == nodeRecord.pCell)
				{
					// If connection is cheaper, replace with old connection
					if (currentRecord.estimatedTotalCost < nodeRecord.estimatedTotalCost)
					{
						existingRecord = nodeRecord;
					}
					// Else just leave it
					else
					{
						goToNextConnection = true;
					}

					break;
				}
			}

			// Remove from closedList if necesarry
			if (existingRecord.pCell)
			{
				for (size_t idx{}; idx < closedList.size(); ++idx)
				{
					hasSameNode = closedList[idx].pCell == currentRecord.pCell;
					hasSameConnection = closedList[idx].pPreviousCell == currentRecord.pPreviousCell;

					if (hasSameNode && hasSameConnection)
					{
						closedList.erase(closedList.begin() + idx);
						break;
					}
				}

				// Reset variables
				existingRecord = {};
				existingRecord.pCell = nullptr;
			}

			// Go next if needed
			if (goToNextConnection)
			{
				goToNextConnection = false;
				continue;
			}


			//----------------//
			//--Second Check--//
			//----------------//

			// Check if node is already on openList
			for (auto& nodeRecord : openList)
			{
				if (pConnectedCell == nodeRecord.pCell)
				{
					// If connection is cheaper, replace with old connection
					if (currentRecord.estimatedTotalCost < nodeRecord.estimatedTotalCost)
					{
						existingRecord = nodeRecord;
					}
					// Else just leave it
					else
					{
						goToNextConnection = true;
					}

					break;
				}
			}

			// Remove from openList if necesarry
			if (existingRecord.pCell)
			{
				for (size_t idx{}; idx < openList.size(); ++idx)
				{
					hasSameNode = openList[idx].pCell == currentRecord.pCell;
					hasSameConnection = openList[idx].pPreviousCell == currentRecord.pPreviousCell;

					if (hasSameNode && hasSameConnection)
					{
						openList.erase(openList.begin() + idx);
						break;
					}
				}

				// Reset variables
				existingRecord = {};
				existingRecord.pCell = nullptr;
			}

			// Go next if needed
			if (goToNextConnection)
			{
				goToNextConnection = false;
				continue;
			}


			//-------------------//
			//--Add to openList--//
			//-------------------//

			CellRecord newRecord;
			newRecord.pCell = pConnectedCell;
			newRecord.pPreviousCell = currentRecord.pCell;
			newRecord.costSoFar = currentRecord.costSoFar + 1;
			newRecord.estimatedTotalCost = GetHeuristicCost(newRecord.pCell, pPlayerCell) + newRecord.costSoFar;

			openList.push_back(newRecord);
		}

		// Remove from openList and add to closedList
		for (size_t idx{}; idx < openList.size(); ++idx)
		{
			hasSameNode = openList[idx].pCell == currentRecord.pCell;
			hasSameConnection = openList[idx].pPreviousCell == currentRecord.pPreviousCell;

			if (hasSameNode && hasSameConnection)
			{
				openList.erase(openList.begin() + idx);
				break;
			}
		}
		closedList.push_back(currentRecord);
	}


	//--------------------//
	//--Reconstruct Path--//
	//--------------------//

	// Do until currentNode is startNode
	while (currentRecord.pCell != pStartCell)
	{
		// Add node to path
		path.push_back(currentRecord.pCell);

		for (auto& records : closedList)
		{
			// TrackBack
			if (records.pCell == currentRecord.pPreviousCell)
			{
				currentRecord = records;
				break;
			}
		}
	}

	path.push_back(pStartCell);
	std::reverse(path.begin(), path.end());
	return path;
}
void enemy::ChaseState::FollowPath(EnemyComponent* pEnemy, float deltaTime)
{
	// Get cells
	// ---------
	grid::GridComponent* pGrid{ pEnemy->GetGrid() };

	// Own
	const glm::vec3 currentPos{ pEnemy->GetGameObject()->GetWorldPosition() };
	grid::Cell* pCurrentCell{ pGrid->GetCell(currentPos) };

	// Next cell
	grid::Cell* pNextCell{ m_DesiredPath[0] };

	// Check if cell reached
	// ---------------------
	if (pCurrentCell == pNextCell)
	{
		// Remove first cell
		m_DesiredPath.pop_front();

		// If no more path, get new one
		if (m_DesiredPath.size() == 0)
		{
			m_DesiredPath = CalculatePath(pEnemy);
		}	

		// Set new nextCell
		pNextCell = m_DesiredPath[0];
	}

	// Go to next cell
	// ---------------
	glm::vec2 moveDirection{};
	auto relativeDirection{ grid::RelativeDirection(pCurrentCell, pNextCell) };

	// Get movementDirection
	switch (relativeDirection)
	{
	case grid::Up:
		moveDirection = glm::vec2{ 0.f, -1.f };
		break;

	case grid::Down:
		moveDirection = glm::vec2{ 0.f, 1.f };
		break;

	case grid::Left:
		moveDirection = glm::vec2{ -1.f, 0.f };
		break;

	case grid::Right:
		moveDirection = glm::vec2{ 1.f, 0.f };
		break;
	}

	// Set movementDirection and move
	m_pMoveCommand->SetMovementDirection(moveDirection);
	m_pMoveCommand->Execute(deltaTime);
}

float enemy::ChaseState::GetHeuristicCost(grid::Cell* pStartNode, grid::Cell* pEndNode) const
{
	const glm::vec2 toDestination{ pEndNode->centerPosition - pStartNode->centerPosition };
	return std::max(abs(toDestination.x), abs(toDestination.y));
}