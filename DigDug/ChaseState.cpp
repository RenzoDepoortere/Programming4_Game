#include "ChaseState.h"

#include "EnemyComponent.h"
#include "GridComponent.h"
#include "GridHelpers.h"
#include "CharacterComponent.h"
#include "GameObject.h"

#include "DigDugSceneManager.h"

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
	for (const auto& currentCharacter : digdug::DigDugSceneManager::GetInstance().GetCharacters())
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
	grid::GridComponent* pGrid{ digdug::DigDugSceneManager::GetInstance().GetGrid() };

	m_pMoveCommand = std::make_unique<dae::MoveCommand>(pEnemyGameObject, glm::vec2{ 0, 1 }, behaviorData.movementSpeed, pGrid);

	// Calculate path
	// --------------
	if (m_pCharacterToChase == nullptr) return;

	auto desiredPos{ m_pCharacterToChase->GetGameObject()->GetWorldPosition() };
	m_DesiredPath = grid::CalculatePath(currentPos, desiredPos, pGrid);

	m_CheckInterval = behaviorData.detectionInterval;
	m_CurrentTime = m_CheckInterval;
	m_CurrentAttackTime = 0.f;
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

		// Calculate path
		auto pGrid{ digdug::DigDugSceneManager::GetInstance().GetGrid() };
		auto currentPos{ pEnemy->GetGameObject()->GetWorldPosition() };
		auto desiredPos{ m_pCharacterToChase->GetGameObject()->GetWorldPosition() };
		m_DesiredPath = grid::CalculatePath(currentPos, desiredPos, pGrid);
	}

	// Follow path
	FollowPath(pEnemy, deltaTime);

	// Check attack
	EnemyStates state{ NR_STATES };
	if (pEnemy->GetBehaviorData().enemyType == enemy::Fygar)
	{
		state = HandleAttack(pEnemy, deltaTime);
		return state;
	}

	// Return
	return NR_STATES;
}

void enemy::ChaseState::FollowPath(EnemyComponent* pEnemy, float deltaTime)
{
	// Get cells
	// ---------
	grid::GridComponent* pGrid{ digdug::DigDugSceneManager::GetInstance().GetGrid() };

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
			auto desiredPos{ m_pCharacterToChase->GetGameObject()->GetWorldPosition() };
			m_DesiredPath = grid::CalculatePath(currentPos, desiredPos, pGrid);

			return;
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

enemy::EnemyStates enemy::ChaseState::HandleAttack(EnemyComponent* pEnemy, float deltaTime)
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