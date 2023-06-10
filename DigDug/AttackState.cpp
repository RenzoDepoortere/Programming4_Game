#include "AttackState.h"

#include "EnemyComponent.h"
#include "AnimationComponent.h"

#include "ResourceManager.h"

enemy::AttackState::AttackState()
{
	// Create textures
	// ---------------
	m_pPrepareTexture = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Characters/Enemies/Fygar/Attacking.png");
	m_pFygarFireTexture = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Characters/Enemies/Fygar/Fire.png");

	// Create fire object
	// ------------------
}

void enemy::AttackState::OnEnter(EnemyComponent* pEnemy)
{
	// Set texture
	// -----------
	m_pAnimationComponent = pEnemy->GetAnimationComponent();

	m_pAnimationComponent->SetTexture(m_pPrepareTexture);
	m_pAnimationComponent->SetSingleSpriteSize(25.f);

	m_pAnimationComponent->SetMaxFrames(2);
	m_pAnimationComponent->SetFramesPerSecond(8);

	m_pAnimationComponent->SetPaused(false);

	// Reset phase
	// -----------
	m_CurrentPhase = Prepare;
	m_CurrentTime = 0.f;
}

void enemy::AttackState::OnLeave(EnemyComponent* /*pEnemy*/)
{
}

enemy::EnemyStates enemy::AttackState::Update(EnemyComponent* pEnemy, float deltaTime)
{
	enemy::EnemyStates state{ NR_STATES };

	switch (m_CurrentPhase)
	{
	case enemy::AttackState::Prepare:
		PrepareAttack(pEnemy, deltaTime);
		break;
	case enemy::AttackState::Attack:
		HandleAttack(deltaTime);
		break;
	case enemy::AttackState::Stun:
		state = HoldStun(pEnemy, deltaTime);
		break;
	}

	return state;
}

void enemy::AttackState::PrepareAttack(EnemyComponent* pEnemy, float deltaTime)
{
	// Prepare
	m_CurrentTime += deltaTime;
	if (pEnemy->GetBehaviorData().attackCharge <= m_CurrentTime)
	{
		m_CurrentTime = 0.f;

		// Pause animation
		m_pAnimationComponent->SetFrame(1);
		m_pAnimationComponent->SetPaused(true);

		// Enable fire

		// Set to attacking
		m_CurrentPhase = Attack;
	}
}
void enemy::AttackState::HandleAttack(float deltaTime)
{
	// Hold for x time
	const float threshold{ 0.5f };
	m_CurrentTime += deltaTime;
	if (threshold <= m_CurrentTime)
	{
		m_CurrentTime = 0.f;

		// Change frame
		m_pAnimationComponent->SetFrame(0);

		// Disable fire

		// Set to stun
		m_CurrentPhase = Stun;
	}
}
enemy::EnemyStates enemy::AttackState::HoldStun(EnemyComponent* pEnemy, float deltaTime)
{
	// Stay for moment
	m_CurrentTime += deltaTime;
	if (pEnemy->GetBehaviorData().attackStun <= m_CurrentTime)
	{
		// Return to roaming
		return Roaming;
	}

	return NR_STATES;
}