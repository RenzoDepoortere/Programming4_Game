#include "RoamingState.h"

#include "EnemyComponent.h"
#include "CharacterComponent.h"
#include "GridComponent.h"
#include "GridHelpers.h"
#include "AnimationComponent.h"

#include "DigDugSceneManager.h"

#include "GameObject.h"
#include "InputMapper.h"
#include "ResourceManager.h"
#include "EventManager.h"
#include "EventsEnum.h"

enemy::RoamingState::RoamingState()
{
	// Create walking sprite
	m_pPookaWalkingAnimation = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Characters/Enemies/Pooka/Pooka_Walk_Animation.png");
	m_pFygarWalkingAnimation = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Characters/Enemies/Fygar/Walking.png");
}

void enemy::RoamingState::OnEnter(EnemyComponent* pEnemy)
{
	// Reset variables
	// ---------------
	m_CurrentRoamTime = 0.f;
	m_CurrentAttackTime = 0.f;

	m_WantedToAttack = false;
	m_WantedToGhost = false;

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
	int fps{ 12 };
	if (pEnemy->GetBehaviorData().enemyType != Pooka)
	{
		pTexture = m_pFygarWalkingAnimation;
		textureSize = 21.f;
		fps = 8;
	}
	
	pAnimationComponent->SetTexture(pTexture);
	pAnimationComponent->SetSingleSpriteSize(textureSize);

	pAnimationComponent->SetMaxFrames(2);
	pAnimationComponent->SetFramesPerSecond(fps);

	pAnimationComponent->SetPaused(false);

	// Subscribe to events
	// -------------------

	if (pEnemy->GetIsControlled())
	{
		dae::EventManager<float>::GetInstance().Subscribe(event::ControllerLeft_1, this);
		dae::EventManager<float>::GetInstance().Subscribe(event::ControllerRight_1, this);
		dae::EventManager<float>::GetInstance().Subscribe(event::ControllerUp_1, this);
		dae::EventManager<float>::GetInstance().Subscribe(event::ControllerDown_1, this);

		dae::EventManager<float>::GetInstance().Subscribe(event::ControllerActionA_1, this);
		dae::EventManager<float>::GetInstance().Subscribe(event::ControllerActionB_1, this);
	}
}
void enemy::RoamingState::OnLeave(EnemyComponent* pEnemy)
{
	// Unsubscribe to events
	if (dae::EventManager<float>::GetIsDestroyed() == false && pEnemy->GetIsControlled())
	{
		dae::EventManager<float>::GetInstance().Unsubscribe(event::ControllerLeft_1, this);
		dae::EventManager<float>::GetInstance().Unsubscribe(event::ControllerRight_1, this);
		dae::EventManager<float>::GetInstance().Unsubscribe(event::ControllerUp_1, this);
		dae::EventManager<float>::GetInstance().Unsubscribe(event::ControllerDown_1, this);

		dae::EventManager<float>::GetInstance().Unsubscribe(event::ControllerActionA_1, this);
		dae::EventManager<float>::GetInstance().Unsubscribe(event::ControllerActionB_1, this);
	}
}

enemy::EnemyStates enemy::RoamingState::Update(EnemyComponent* pEnemy, float deltaTime)
{
	// Variables
	EnemyStates state{ NR_STATES };
	const bool isControlled{ pEnemy->GetIsControlled() };

	// Update if not controlled
	if (isControlled == false)
	{
		// Check ghost timer
		state = CheckGhostTimer(pEnemy, deltaTime);
		if (state != NR_STATES) return state;

		// Check attack
		if (pEnemy->GetBehaviorData().enemyType == enemy::Fygar)
		{
			state = HandleAttack(pEnemy, deltaTime);
			if (state != NR_STATES) return state;
		}

		// Path
		HandlePathing(pEnemy, deltaTime);
		state = LookForPlayer(pEnemy, deltaTime);
	}
	// If controlled, and wanted to attack
	else if (isControlled && m_WantedToAttack)
	{
		state = Attack;
	}

	// Return
	return state;
}

void enemy::RoamingState::HandleEvent(unsigned int eventID, float deltaTime)
{
	// Check event
	glm::vec2 movementDirection{};
	switch (eventID)
	{
	case event::ControllerLeft_1:
		movementDirection = glm::vec2{ -1.f, 0.f };
		break;

	case event::ControllerRight_1:
		movementDirection = glm::vec2{ 1.f, 0.f };
		break;

	case event::ControllerUp_1:
		movementDirection = glm::vec2{ 0.f, -1.f };
		break;

	case event::ControllerDown_1:
		movementDirection = glm::vec2{ 0.f, 1.f };
		break;

	case event::ControllerActionA_1:
		
		break;

	case event::ControllerActionB_1:
		m_WantedToAttack = true;
		break;
	}

	// Handle input
	m_pMoveCommand->SetMovementDirection(movementDirection);
	m_pMoveCommand->Execute(deltaTime);
}
void enemy::RoamingState::OnSubjectDestroy()
{
}

void enemy::RoamingState::InitMovementCommands(EnemyComponent* pEnemy)
{
	m_CommandInitialized = true;

	// Variables
	grid::GridComponent* pGrid{ digdug::DigDugSceneManager::GetInstance().GetGrid() };
	dae::GameObject* pGameObject{ pEnemy->GetGameObject() };
	const float movementSpeed{ pEnemy->GetBehaviorData().movementSpeed };

	// Create command
	m_pMoveCommand = std::make_unique<dae::MoveCommand>(pGameObject, glm::vec2{}, movementSpeed, pGrid, true);
}

enemy::EnemyStates enemy::RoamingState::CheckGhostTimer(EnemyComponent* pEnemy, float deltaTime)
{
	auto behaviorData{ pEnemy->GetBehaviorData() };

	m_CurrentRoamTime += deltaTime;
	if (behaviorData.ghostTime <= m_CurrentRoamTime)
	{
		return Ghost;
	}

	return NR_STATES;
}
enemy::EnemyStates enemy::RoamingState::HandleAttack(EnemyComponent* pEnemy, float deltaTime)
{
	// Increase timer
	m_CurrentAttackTime += deltaTime;
	if (pEnemy->GetBehaviorData().attackTime <= m_CurrentAttackTime)
	{
		// Attack
		return Attack;
	}

	// Return
	return NR_STATES;
}

void enemy::RoamingState::HandlePathing(EnemyComponent* pEnemy, float deltaTime)
{
	// Get current cell
	glm::vec3 currentPos{ pEnemy->GetGameObject()->GetWorldPosition() };
	grid::Cell* pStartCell{ digdug::DigDugSceneManager::GetInstance().GetGrid()->GetCell(currentPos)};

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
	m_pMoveCommand->Execute(deltaTime);
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
	glm::vec2 movementDirection{};

	switch (direction)
	{
	case grid::Left:
		movementDirection = glm::vec2{ -1.f, 0.f };
		break;

	case grid::Right:
		movementDirection = glm::vec2{ 1.f, 0.f };
		break;

	case grid::Up:
		movementDirection = glm::vec2{ 0.f, -1.f };
		break;

	case grid::Down:
		movementDirection = glm::vec2{ 0.f, 1.f };
		break;
	}

	m_pMoveCommand->SetMovementDirection(movementDirection);
}

enemy::EnemyStates enemy::RoamingState::LookForPlayer(EnemyComponent* pEnemy, float /*deltaTime*/)
{
	// Get currentCell
	// ---------------
	const glm::vec3 currentPos{ pEnemy->GetGameObject()->GetWorldPosition() };
	grid::GridComponent* pGrid{ digdug::DigDugSceneManager::GetInstance().GetGrid() };
	grid::Cell* pCurrentCell{ pGrid->GetCell(currentPos) };

	const int detectionRange{ pEnemy->GetBehaviorData().detectionRange };

	// Loop through characters
	// -----------------------
	glm::vec3 characterPos{};
	grid::Cell* pCharacterCell{ nullptr };
	
	bool insideXRange{}, insideYRange{};
	bool sameRow{}, sameCol{};
	int cellsBetween{};

	for (const auto& currentCharacter : digdug::DigDugSceneManager::GetInstance().GetCharacters())
	{
		// Get cell
		characterPos = currentCharacter->GetGameObject()->GetWorldPosition();
		pCharacterCell = pGrid->GetCell(characterPos);

		// Check if valid cell
		if (pCharacterCell == nullptr) continue;

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