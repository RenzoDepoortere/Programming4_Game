#include "RoamingState.h"

#include "EnemyComponent.h"
#include "GameObject.h"

#include "GridComponent.h"
#include "InputMapper.h"

void Enemy::RoamingState::OnEnter(EnemyComponent* pEnemy)
{
	// Create move commands
	// --------------------
	if (m_pCurrentCommand == nullptr)
	{
		// Variables
		glm::vec2 movementDirection{};
		dae::GameObject* pGameObject{ pEnemy->GetGameObject() };
		const float movementSpeed{ pEnemy->GetBehaviorData().movementSpeed };
		grid::GridComponent* pGrid{ pEnemy->GetGrid() };

		// Left
		movementDirection = glm::vec2{ -1, 0 };
		std::unique_ptr<dae::MoveCommand> pLeftMoveCommand{ std::make_unique<dae::MoveCommand>(pGameObject, movementDirection, movementSpeed, pGrid, false) };

		// Right
		movementDirection = glm::vec2{ 1, 0 };
		std::unique_ptr<dae::MoveCommand> pRightMoveCommand{ std::make_unique<dae::MoveCommand>(pGameObject, movementDirection, movementSpeed, pGrid, false) };

		// Down
		movementDirection = glm::vec2{ 0, 1 };
		std::unique_ptr<dae::MoveCommand> pDownMoveCommand{ std::make_unique<dae::MoveCommand>(pGameObject, movementDirection, movementSpeed, pGrid, false) };

		// Up
		movementDirection = glm::vec2{ 0, -1 };
		std::unique_ptr<dae::MoveCommand> pUpMoveCommand{ std::make_unique<dae::MoveCommand>(pGameObject, movementDirection, movementSpeed, pGrid, false) };

		// Check if controlled or not
		// --------------------------
		if (pEnemy->GetIsControlled() == false)
		{
			// Store commands
			m_pMoveCommands[static_cast<int>(MovementEnum::Left)] = std::move(pLeftMoveCommand);
			m_pMoveCommands[static_cast<int>(MovementEnum::Right)] = std::move(pRightMoveCommand);
			m_pMoveCommands[static_cast<int>(MovementEnum::Down)] = std::move(pDownMoveCommand);
			m_pMoveCommands[static_cast<int>(MovementEnum::Up)] = std::move(pUpMoveCommand);

			m_pCurrentCommand = m_pMoveCommands[static_cast<int>(MovementEnum::Left)].get();
		}
		else
		{
			// Map command to input
			const auto keyState{ dae::InputMapper::KeyState::Hold };
			const unsigned long controllerID{ pEnemy->GetControllerID() };

			// Left
			auto controllerInput{ std::make_pair(controllerID, dae::InputManager::ControllerButton::DPadLeft) };
			auto inputKeys{ std::make_pair(SDL_SCANCODE_UNKNOWN, controllerInput) };

			dae::InputMapper::GetInstance().MapInputKey(inputKeys, keyState, std::move(pLeftMoveCommand));

			// Right
			controllerInput = std::make_pair(controllerID, dae::InputManager::ControllerButton::DPadRight);
			inputKeys = std::make_pair(SDL_SCANCODE_UNKNOWN, controllerInput);

			dae::InputMapper::GetInstance().MapInputKey(inputKeys, keyState, std::move(pRightMoveCommand));

			// Down
			controllerInput = std::make_pair(controllerID, dae::InputManager::ControllerButton::DPadDown);
			inputKeys = std::make_pair(SDL_SCANCODE_UNKNOWN, controllerInput);

			dae::InputMapper::GetInstance().MapInputKey(inputKeys, keyState, std::move(pDownMoveCommand));

			// Up
			controllerInput = std::make_pair(controllerID, dae::InputManager::ControllerButton::DPadUp);
			inputKeys = std::make_pair(SDL_SCANCODE_UNKNOWN, controllerInput);

			dae::InputMapper::GetInstance().MapInputKey(inputKeys, keyState, std::move(pUpMoveCommand));
		}
	}
}
void Enemy::RoamingState::OnLeave(EnemyComponent* /*pEnemy*/)
{
}

void Enemy::RoamingState::Update(EnemyComponent* pEnemy, float deltaTime)
{
	// Don't update if controlled
	if (pEnemy->GetIsControlled()) return;
	
	HandlePathing(pEnemy, deltaTime);
}

void Enemy::RoamingState::HandlePathing(EnemyComponent* pEnemy, float deltaTime)
{
	// Get current cell
	glm::vec3 currentPos{ pEnemy->GetGameObject()->GetWorldPosition() };
	grid::Cell* pStartCell{ pEnemy->GetGrid()->GetCell(currentPos)};

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
	// ---------------------
	m_pCurrentCommand->Execute(deltaTime);
}
void Enemy::RoamingState::FindNextCell(grid::Cell* pCurrentCell)
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

	// If not same row
	if (notSameRow)
	{
		// Row is lower
		if (lowerRow)	m_pCurrentCommand = m_pMoveCommands[static_cast<int>(MovementEnum::Up)].get();
		// Row is higher
		else			m_pCurrentCommand = m_pMoveCommands[static_cast<int>(MovementEnum::Down)].get();
	}
	// If not same col
	else
	{
		// Col is lower
		if (lowerCol)	m_pCurrentCommand = m_pMoveCommands[static_cast<int>(MovementEnum::Left)].get();
		// Col is higher
		else			m_pCurrentCommand = m_pMoveCommands[static_cast<int>(MovementEnum::Right)].get();
	}
}