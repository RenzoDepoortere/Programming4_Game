#include "GhostState.h"

#include "EnemyComponent.h"
#include "AnimationComponent.h"
#include "GameObject.h"
#include "CharacterComponent.h"
#include "GridComponent.h"

#include "FirstScene.h"

#include "ResourceManager.h"

enemy::GhostState::GhostState()
{
	// Create textures
	m_pPookaGhostTexture = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Characters/Enemies/Pooka/Ghost_Animation.png");
	m_pFygarGhostTexture = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Characters/Enemies/Fygar/Ghost_Animation.png");
}

void enemy::GhostState::OnEnter(EnemyComponent* pEnemy)
{
	// Reset variables
	m_CurrentGhostTime = 0.f;
	m_AllowToTransferBack = false;

	// Create movementCommand
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
}
void enemy::GhostState::OnLeave(EnemyComponent* /*pEnemy*/)
{
}

enemy::EnemyStates enemy::GhostState::Update(EnemyComponent* pEnemy, float deltaTime)
{
	HandleMovement(pEnemy, deltaTime);
	EnemyStates state = HandleGoingBack(pEnemy, deltaTime);

	return state;
}

void enemy::GhostState::HandleMovement(EnemyComponent* pEnemy, float deltaTime)
{
	const auto currentPos{ pEnemy->GetGameObject()->GetWorldPosition() };

	// Get closest player
	float closestDistance{ FLT_MAX };
	float currentDistance{};
	glm::vec3 desiredPos{};
	glm::vec3 playerPos{};

	for (const auto& currentPlayer : pEnemy->GetCharacters())
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
	grid::GridComponent* pGrid{ FirstScene::GetInstance().GetGrid() };

	const glm::vec3 currentPos{ pEnemy->GetGameObject()->GetWorldPosition() };
	grid::Cell* pCurrentCell{ pGrid->GetCell(currentPos) };

	if (pCurrentCell->textureID == 0) return Roaming;
	else							  return NR_STATES;
}