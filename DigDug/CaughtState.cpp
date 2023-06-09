#include "CaughtState.h"

#include "AnimationComponent.h"
#include "EnemyComponent.h"
#include "GameObject.h"

#include "ResourceManager.h"

enemy::CaughtState::CaughtState()
{
	// Create caught textures
	auto pTexture = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Characters/Enemies/Pooka/CaughtStates/Blow_State_1.png");
	float textureSize = 24.f;
	m_pCaughtTextures[0] = std::make_pair(pTexture, textureSize);

	pTexture = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Characters/Enemies/Pooka/CaughtStates/Blow_State_2.png");
	textureSize = 33.f;
	m_pCaughtTextures[1] = std::make_pair(pTexture, textureSize);

	pTexture = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Characters/Enemies/Pooka/CaughtStates/Blow_State_3.png");
	textureSize = 34.f;
	m_pCaughtTextures[2] = std::make_pair(pTexture, textureSize);

	pTexture = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Characters/Enemies/Pooka/CaughtStates/Blow_State_4.png");
	textureSize = 41.f;
	m_pCaughtTextures[3] = std::make_pair(pTexture, textureSize);
}

void enemy::CaughtState::OnEnter(EnemyComponent* pEnemy)
{
	// Set variables
	m_CurrentState = 0;
	m_CurrentTime = 0;

	// Set caught texture
	m_pAnimationComponent = pEnemy->GetAnimationComponent();

	m_pAnimationComponent->SetTexture(m_pCaughtTextures[m_CurrentState].first);
	m_pAnimationComponent->SetSingleSpriteSize(m_pCaughtTextures[m_CurrentState].second);
	
	m_pAnimationComponent->SetMaxFrames(1);
	m_pAnimationComponent->SetFramesPerSecond(12);

	m_pAnimationComponent->SetPaused(true);
}
void enemy::CaughtState::OnLeave(EnemyComponent* /*pEnemy*/)
{
}

enemy::EnemyStates enemy::CaughtState::Update(EnemyComponent* pEnemy, float deltaTime)
{
	EnemyStates state{};
	state = HandleCaughtGrading(pEnemy, deltaTime);

	return state;
}

enemy::EnemyStates enemy::CaughtState::HandleCaughtGrading(EnemyComponent* pEnemy, float deltaTime)
{
	m_CurrentTime += deltaTime;

	// If was last state
	const bool lastState{ m_CurrentState == m_pCaughtTextures.size() - 1 };
	if (lastState)
	{
		// Check if reached hold treshold
		const float treshold{ 0.5f };
		if (treshold <= m_CurrentTime)
		{
			// Set inactive
			pEnemy->GetGameObject()->SetIsActive(false);
			pEnemy->GetGameObject()->SetIsHidden(true);
		}

		// Return
		return NR_STATES;
	}

	// Check if was blown
	const bool wasBlown{ pEnemy->GetBlown() };
	if (wasBlown)
	{
		pEnemy->SetBlow(false);

		// Go up 1 state
		++m_CurrentState;

		// Change sprite
		m_pAnimationComponent->SetTexture(m_pCaughtTextures[m_CurrentState].first);
		m_pAnimationComponent->SetSingleSpriteSize(m_pCaughtTextures[m_CurrentState].second);

		// Reset timer
		m_CurrentTime = 0.f;

		// Return
		return NR_STATES;
	}


	// Check if reached go back treshold
	const float treshold{ 1.f };
	if (treshold <= m_CurrentTime)
	{
		// Go down 1 state
		m_CurrentState--;

		// If negative state
		if (m_CurrentState <= -1)
		{
			// Go back to normal
			pEnemy->SetCaught(false);
			return Roaming;
		}

		// Change sprite
		m_pAnimationComponent->SetTexture(m_pCaughtTextures[m_CurrentState].first);
		m_pAnimationComponent->SetSingleSpriteSize(m_pCaughtTextures[m_CurrentState].second);

		// Reset timer
		m_CurrentTime = 0.f;
	}

	return NR_STATES;
}