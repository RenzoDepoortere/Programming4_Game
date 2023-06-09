#include "FleeState.h"

#include "EnemyComponent.h"
#include "GridHelpers.h"
#include "GameObject.h"
#include "AnimationComponent.h"

#include "FirstScene.h"

#include "ResourceManager.h"

enemy::FleeState::FleeState()
{
	// Create walking sprite again
	m_pPookaWalkingAnimation = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Characters/Enemies/Pooka/Pooka_Walk_Animation.png");
	m_pFygarWalkingAnimation = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Characters/Enemies/Fygar/Walking.png");
}

void enemy::FleeState::OnEnter(EnemyComponent* pEnemy)
{
	// Create moveCommand
	// ------------------
	if (m_pMoveCommand == nullptr)
	{
		const auto behaviorData{ pEnemy->GetBehaviorData() };
		m_pMoveCommand = std::make_unique<dae::MoveCommand>(pEnemy->GetGameObject(), glm::vec2{}, behaviorData.movementSpeed, FirstScene::GetInstance().GetGrid());
	}

	// Set texture
	// -----------
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

	// Calculate path
	// --------------
	auto pGrid{ FirstScene::GetInstance().GetGrid() };
	auto currentPos{ pEnemy->GetGameObject()->GetWorldPosition() };
	const glm::vec2 centerPos{ pGrid->GetCell(0)->centerPosition };
	const glm::vec3 desiredPos{ centerPos.x, centerPos.y, 0.f };

	m_pPathToFollow = grid::CalculatePath(currentPos, desiredPos, pGrid);
}
void enemy::FleeState::OnLeave(EnemyComponent* /*pEnemy*/)
{
}

enemy::EnemyStates enemy::FleeState::Update(EnemyComponent* pEnemy, float deltaTime)
{
	FollowPath(pEnemy, deltaTime);

	return NR_STATES;
}

void enemy::FleeState::FollowPath(EnemyComponent* pEnemy, float deltaTime)
{
	// Return if no path to follow
	if (m_pPathToFollow.size() == 0) return;

	// Get cells
	// ---------
	grid::GridComponent* pGrid{ FirstScene::GetInstance().GetGrid() };

	// Own
	const glm::vec3 currentPos{ pEnemy->GetGameObject()->GetWorldPosition() };
	grid::Cell* pCurrentCell{ pGrid->GetCell(currentPos) };

	// Next cell
	grid::Cell* pNextCell{ m_pPathToFollow[0] };

	// Check if cell reached
	// ---------------------
	if (pCurrentCell == pNextCell)
	{
		// Remove first cell
		m_pPathToFollow.pop_front();

		// If no more path, handle it
		if (m_pPathToFollow.size() == 0)
		{
			HandleReachingFinish(pEnemy);
			return;
		}

		// Set new nextCell
		pNextCell = m_pPathToFollow[0];
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

void enemy::FleeState::HandleReachingFinish(EnemyComponent* /*pEnemy*/)
{

}