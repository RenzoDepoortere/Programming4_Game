#include "GhostState.h"

#include "EnemyComponent.h"
#include "AnimationComponent.h"
#include "GameObject.h"
#include "CharacterComponent.h"
#include "GridComponent.h"

#include "DigDugSceneManager.h"

#include "ResourceManager.h"
#include "EventManager.h"
#include "EventsEnum.h"

enemy::GhostState::GhostState()
{
	// Create textures
	m_pPookaGhostTexture = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Characters/Enemies/Pooka/Ghost_Animation.png");
	m_pFygarGhostTexture = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Characters/Enemies/Fygar/Ghost_Animation.png");
}

void enemy::GhostState::OnEnter(EnemyComponent* pEnemy)
{
	// Reset variables
	// ---------------
	m_CurrentGhostTime = 0.f;
	m_AllowToTransferBack = false;

	m_WantsToGoBack = false;
	m_pEnemyComponent = pEnemy;

	// Create movementCommand
	// ----------------------
	auto behaviorData{ pEnemy->GetBehaviorData() };
	if (m_pMoveCommand == nullptr)
	{
		m_pMoveCommand = std::make_unique<dae::MoveCommand>(pEnemy->GetGameObject(), glm::vec2{}, behaviorData.movementSpeed / 2.f);
	}
	else
	{
		m_pMoveCommand->SetMovementSpeed(behaviorData.movementSpeed / 2.f);
	}

	// Set texture
	// -----------
	auto pAnimationComponent = pEnemy->GetAnimationComponent();

	auto pTexture = m_pPookaGhostTexture;
	float textureSize = 19.f;
	if (pEnemy->GetBehaviorData().enemyType != Pooka)
	{
		pTexture = m_pFygarGhostTexture;
		textureSize = 21.f;
	}

	pAnimationComponent->SetTexture(pTexture);
	pAnimationComponent->SetSingleSpriteSize(textureSize);

	pAnimationComponent->SetMaxFrames(2);
	pAnimationComponent->SetFramesPerSecond(6);

	// Subscribe to events
	// -------------------
	if (pEnemy->GetIsControlled())
	{
		dae::EventManager<float>::GetInstance().Subscribe(event::ControllerLeft_1, this);
		dae::EventManager<float>::GetInstance().Subscribe(event::ControllerRight_1, this);
		dae::EventManager<float>::GetInstance().Subscribe(event::ControllerUp_1, this);
		dae::EventManager<float>::GetInstance().Subscribe(event::ControllerDown_1, this);

		dae::EventManager<float>::GetInstance().Subscribe(event::ControllerActionA_1, this);
	}
}
void enemy::GhostState::OnLeave(EnemyComponent* pEnemy)
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

enemy::EnemyStates enemy::GhostState::Update(EnemyComponent* pEnemy, float deltaTime)
{
	EnemyStates state{ NR_STATES };

	// Auto if not being controlled
	if (pEnemy->GetIsControlled() == false)
	{
		HandleMovement(pEnemy, deltaTime);
		state = HandleGoingBack(pEnemy, deltaTime);
	}
	// Else
	else if (m_WantsToGoBack)
	{
		state = Roaming;
	}

	return state;
}

void enemy::GhostState::HandleEvent(unsigned int eventID, float deltaTime)
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

		// Check if on empty cell
		grid::GridComponent* pGrid{ digdug::DigDugSceneManager::GetInstance().GetGrid() };

		const glm::vec3 currentPos{ m_pEnemyComponent->GetGameObject()->GetWorldPosition() };
		grid::Cell* pCurrentCell{ pGrid->GetCell(currentPos) };

		if (pCurrentCell->textureID == 0)
		{
			m_WantsToGoBack = true;
		}
		break;
	}

	// Handle input
	m_pMoveCommand->SetMovementDirection(movementDirection);
	m_pMoveCommand->Execute(deltaTime);
}
void enemy::GhostState::OnSubjectDestroy()
{
}

void enemy::GhostState::HandleMovement(EnemyComponent* pEnemy, float deltaTime)
{
	const auto currentPos{ pEnemy->GetGameObject()->GetWorldPosition() };

	// Get closest player
	float closestDistance{ FLT_MAX };
	float currentDistance{};
	glm::vec3 desiredPos{};
	glm::vec3 playerPos{};

	for (const auto& currentPlayer : digdug::DigDugSceneManager::GetInstance().GetCharacters())
	{
		playerPos = currentPlayer->GetGameObject()->GetWorldPosition();
		currentDistance = utils::GetSqrdMagnitude(playerPos - currentPos);

		if (currentDistance <= closestDistance)
		{
			closestDistance = currentDistance;
			desiredPos = playerPos;
		}
	}

	// Set direction to closest player
	glm::vec3 direction{ playerPos - currentPos };
	direction = utils::GetNormalizedVector(direction);

	// Move towards direction
	m_pMoveCommand->SetMovementDirection({ direction.x, direction.y });
	m_pMoveCommand->Execute(deltaTime);
}
enemy::EnemyStates enemy::GhostState::HandleGoingBack(EnemyComponent* pEnemy, float deltaTime)
{
	// Check if able to transfer back
	m_CurrentGhostTime += deltaTime;
	auto behaviorData{ pEnemy->GetBehaviorData() };
	if (behaviorData.maxGhostTime <= m_CurrentGhostTime)
	{
		m_AllowToTransferBack = true;
	}

	// Return if can't transfer back
	if (m_AllowToTransferBack == false) return NR_STATES;

	// Check if on empty cell
	grid::GridComponent* pGrid{ digdug::DigDugSceneManager::GetInstance().GetGrid() };

	const glm::vec3 currentPos{ pEnemy->GetGameObject()->GetWorldPosition() };
	grid::Cell* pCurrentCell{ pGrid->GetCell(currentPos) };

	if (pCurrentCell->textureID == 0)
	{
		if (pEnemy->GetIsFleeing()) return Flee;
		else						return Roaming;
	}
	else							  return NR_STATES;
}