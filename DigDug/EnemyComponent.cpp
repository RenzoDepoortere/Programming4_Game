#include "EnemyComponent.h"

#include "GameObject.h"
#include "GridComponent.h"

#include "Utils.h"

EnemyComponent::EnemyComponent(dae::GameObject* pParentObject)
	: Component{ pParentObject }
{
}

void EnemyComponent::Update(float deltaTime)
{
	// Update currentState
	//m_pCurrentState->Update(deltaTime);

	TempPathing(deltaTime);
}

void EnemyComponent::CreateMovementCommand()
{
	// Create move command
	const glm::vec2 direction{ 0, 1 };
	m_pMoveCommand = std::make_unique<dae::MoveCommand>(GetGameObject(), direction, m_EnemyBehavior.movementSpeed, m_pGrid, false);
}

void EnemyComponent::TempPathing(float deltaTime)
{
	// Get current cell
	glm::vec3 currentPos{ GetGameObject()->GetWorldPosition() };
	grid::Cell* pStartCell{ m_pGrid->GetCell(currentPos) };

	// Search next cell
	// ----------------

	// Check if close enough to nextCell
	bool closeEnoughToTarget{ false };
	if (m_pNextCell == pStartCell)
	{
		const glm::vec3 nextCenterPos{ m_pNextCell->centerPosition.x, m_pNextCell->centerPosition.y, 0.f };
		const glm::vec3 directionTo{ nextCenterPos - currentPos };
		
		const float length{ utils::GetSqrdMagnitude(directionTo) };
		const float minDistance{ 50 };

		if (length <= minDistance) closeEnoughToTarget = true;
	}

	// Search next cell if necessary
	if (m_pNextCell == nullptr || closeEnoughToTarget)
	{
		FindNextCell(pStartCell);
	}

	// Move towards nextCell
	// --------------------
	m_pMoveCommand->Execute(deltaTime);

	//// Set previousCell
	//// ----------------
	//currentPos = GetGameObject()->GetWorldPosition();
	//grid::Cell* pCurrentCell{ m_pGrid->GetCell(currentPos) };

	//if (pCurrentCell != pStartCell) 
}
void EnemyComponent::FindNextCell(grid::Cell* pCurrentCell)
{
	// Get random other connected cell
	// -------------------------------

	grid::Cell* pDesiredCell{ nullptr };
	auto& connectionConnections{ pCurrentCell->pConnectedCells };

	// If only 1 connection
	if (connectionConnections.size() == 1)
	{
		pDesiredCell = connectionConnections[0];
	}
	// Multiple connections
	else
	{
		// Get vector without previousCell
		std::vector<grid::Cell*> pAvailableCells{ pCurrentCell->pConnectedCells };
		std::erase(pAvailableCells, m_pPreviousCell);

		// Get random cell
		const int randomIdx{ rand() % static_cast<int>(pAvailableCells.size()) };
		pDesiredCell = pAvailableCells[randomIdx];
	}
	

	// Set variables
	// -------------
	m_pNextCell = pDesiredCell;
	m_pPreviousCell = pCurrentCell;

	// Calculate next direction
	// ------------------------
	const bool lowerRow{ m_pNextCell->rowCol.x < pCurrentCell->rowCol.x };
	const bool lowerCol{ m_pNextCell->rowCol.y < pCurrentCell->rowCol.y };
	
	const bool notSameRow{ m_pNextCell->rowCol.x != pCurrentCell->rowCol.x };

	glm::vec2 movementDirection{};

	// If not same row
	if (notSameRow)
	{
		// Row is lower
		if (lowerRow)	movementDirection = glm::vec2{ 0, -1 };
		// Row is higher
		else			movementDirection = glm::vec2{ 0, 1 };
	}
	// If not same col
	else
	{
		// Col is lower
		if (lowerCol)	movementDirection = glm::vec2{ -1, 0 };
		// Col is higher
		else			movementDirection = glm::vec2{ 1, 0 };
	}

	// Set direction
	m_pMoveCommand->SetMovementDirection(movementDirection);
}