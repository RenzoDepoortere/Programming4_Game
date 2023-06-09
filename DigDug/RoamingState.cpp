#include "RoamingState.h"

#include "EnemyComponent.h"
#include "CharacterComponent.h"
#include "GridComponent.h"
#include "GridHelpers.h"
#include "AnimationComponent.h"

#include "GameObject.h"
#include "InputMapper.h"
#include "ResourceManager.h"

enemy::RoamingState::RoamingState()
{
	// Create walking sprite
	m_pPookaWalkingAnimation = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Characters/Enemies/Pooka/Pooka_Walk_Animation.png");
	m_pFygarWalkingAnimation = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Characters/Enemies/Fygar/Walking.png");
}

void enemy::RoamingState::OnEnter(EnemyComponent* pEnemy)
{
	// Create move commands
	// --------------------
	if (m_CommandInitialized == false)
	{
		InitMovementCommands(pEnemy);
	}

	// Set animation sprite
	// --------------------
	dae::AnimationComponent* pAnimationComponent{ pEnemy->GetAnimationComponent() };

	auto pTexture{ m_pPookaWalkingAnimation };
	float textureSize{ 25.f };
	if (pEnemy->GetBehaviorData().enemyType != Pooka)
	{
		pTexture = m_pFygarWalkingAnimation;
		textureSize = 21.5f;
	}
	
	pAnimationComponent->SetTexture(pTexture);
	pAnimationComponent->SetSingleSpriteSize(textureSize);

	pAnimationComponent->SetMaxFrames(2);
	pAnimationComponent->SetFramesPerSecond(12);

	pAnimationComponent->SetPaused(false);
}
void enemy::RoamingState::OnLeave(EnemyComponent* /*pEnemy*/)
{
}

enemy::EnemyStates enemy::RoamingState::Update(EnemyComponent* pEnemy, float deltaTime)
{
	// Variables
	EnemyStates state{};
	const bool isControlled{ pEnemy->GetIsControlled() };

	// Update if not controlled
	if (isControlled == false)
	{
		HandlePathing(pEnemy, deltaTime);
		state = LookForPlayer(pEnemy, deltaTime);
	}
	//// If controlled, check for input
	//else
	//{
	//	// Attack if X pressed
	//	if (dae::InputManager::GetInstance().IsPressed(pEnemy->GetControllerID(), dae::InputManager::ControllerButton::ButtonX))
	//	{

	//	}
	//}

	// Return
	return state;
}

void enemy::RoamingState::InitMovementCommands(EnemyComponent* pEnemy)
{
	m_CommandInitialized = true;

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
		m_pMoveCommands[static_cast<int>(grid::CellRelativeDirection::Left)] = std::move(pLeftMoveCommand);
		m_pMoveCommands[static_cast<int>(grid::CellRelativeDirection::Right)] = std::move(pRightMoveCommand);
		m_pMoveCommands[static_cast<int>(grid::CellRelativeDirection::Down)] = std::move(pDownMoveCommand);
		m_pMoveCommands[static_cast<int>(grid::CellRelativeDirection::Up)] = std::move(pUpMoveCommand);

		m_pCurrentCommand = m_pMoveCommands[static_cast<int>(grid::CellRelativeDirection::Left)].get();
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

void enemy::RoamingState::HandlePathing(EnemyComponent* pEnemy, float deltaTime)
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
void enemy::RoamingState::FindNextCell(grid::Cell* pCurrentCell)
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
	const grid::CellRelativeDirection direction{ grid::RelativeDirection(pCurrentCell, m_pNextCell) };
	m_pCurrentCommand = m_pMoveCommands[static_cast<int>(direction)].get();
}

enemy::EnemyStates enemy::RoamingState::LookForPlayer(EnemyComponent* pEnemy, float /*deltaTime*/)
{
	// Get currentCell
	// ---------------
	const glm::vec3 currentPos{ pEnemy->GetGameObject()->GetWorldPosition() };
	grid::GridComponent* pGrid{ pEnemy->GetGrid() };
	grid::Cell* pCurrentCell{ pGrid->GetCell(currentPos) };

	const int detectionRange{ pEnemy->GetBehaviorData().detectionRange };

	// Loop through characters
	// -----------------------
	glm::vec3 characterPos{};
	grid::Cell* pCharacterCell{ nullptr };
	
	bool insideXRange{}, insideYRange{};
	bool sameRow{}, sameCol{};
	int cellsBetween{};

	for (const auto& currentCharacter : pEnemy->GetCharacters())
	{
		// Get cell
		characterPos = currentCharacter->GetGameObject()->GetWorldPosition();
		pCharacterCell = pGrid->GetCell(characterPos);

		// Check if in same cell
		if (pCharacterCell == pCurrentCell) return Chase;

		// Check if either same row or col
		sameRow = pCharacterCell->rowCol.x == pCurrentCell->rowCol.x;
		sameCol = pCharacterCell->rowCol.y == pCurrentCell->rowCol.y;
		if ((sameCol || sameRow) == false) continue;

		// Check if inside range
		insideXRange = pCurrentCell->rowCol.x - detectionRange <= pCharacterCell->rowCol.x && pCharacterCell->rowCol.x <= pCurrentCell->rowCol.x + detectionRange;
		insideYRange = pCurrentCell->rowCol.y - detectionRange <= pCharacterCell->rowCol.y && pCharacterCell->rowCol.y <= pCurrentCell->rowCol.y + detectionRange;
		if ((insideXRange && insideYRange) == false) continue;
	
		// Check if no dirt between
		if (sameRow)
		{
			cellsBetween = static_cast<int>(pCharacterCell->rowCol.y - pCurrentCell->rowCol.y);

			if (IsDirtBetween(cellsBetween, false, pGrid, pCurrentCell, pCharacterCell)) continue;
			else																		return Chase;
		}

		if (sameCol)
		{
			cellsBetween = static_cast<int>(pCharacterCell->rowCol.x - pCurrentCell->rowCol.x);
			if (abs(cellsBetween) == 1) return Chase;

			if (IsDirtBetween(cellsBetween, true, pGrid, pCurrentCell, pCharacterCell)) continue;
			else																	     return Chase;
		}
	}

	// Return default if not inside range
	return NR_STATES;
}
bool enemy::RoamingState::IsDirtBetween(int cellsBetween, bool checkRows, grid::GridComponent* pGrid, grid::Cell* pCurrentCell, grid::Cell* pCharacterCell)
{
	// If only a cell apart, return false
	if (abs(cellsBetween) == 1) return false;

	// Loop through cellsBetween
	const int nrCols{ pGrid->GetNrCols() };
	grid::Cell* pCellBetween{ nullptr };

	int moveDirection{ (cellsBetween < 0) ? -1 : 1 };
	bool dirtBetween{ false };
	int cellIdx{};

	for (int idx{ 1 }; idx < abs(cellsBetween); ++idx)
	{
		if (checkRows) cellIdx = static_cast<int>((pCurrentCell->rowCol.x + (idx * moveDirection)) * nrCols + pCharacterCell->rowCol.y);
		else		   cellIdx = static_cast<int>(pCurrentCell->rowCol.x * nrCols + (pCharacterCell->rowCol.y + (idx * moveDirection)));

		pCellBetween = pGrid->GetCell(cellIdx);
		if (pCellBetween == nullptr) continue;

		if (pCellBetween->textureID != 0)
		{
			dirtBetween = true;
			break;
		}
	}

	if (dirtBetween) return true;
	else			 return false;
}