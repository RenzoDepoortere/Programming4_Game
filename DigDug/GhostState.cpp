#include "GhostState.h"

#include "EnemyComponent.h"
#include "AnimationComponent.h"

#include "ResourceManager.h"

enemy::GhostState::GhostState()
{
	// Create textures
	m_pPookaGhostTexture = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Characters/Enemies/Pooka/Ghost_Animation.png");
	m_pFygarGhostTexture = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Characters/Enemies/Fygar/Ghost_Animation.png");
}

void enemy::GhostState::OnEnter(EnemyComponent* pEnemy)
{
	// Create movementCommand
	auto behaviorData{ pEnemy->GetBehaviorData() };
	if (m_pMoveCommand == nullptr)
	{
		glm::vec2 movementDirection{};

		m_pMoveCommand = std::make_unique<dae::MoveCommand>(pEnemy->GetGameObject(), movementDirection, behaviorData.movementSpeed, pEnemy->GetGrid(), false);
	}
	else
	{
		m_pMoveCommand->SetMovementSpeed(behaviorData.movementSpeed);
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

enemy::EnemyStates enemy::GhostState::Update(EnemyComponent* /*pEnemy*/, float /*deltaTime*/)
{
	return NR_STATES;
}