#include "FireComponent.h"

#include "GameObject.h"
#include "RenderTextureComponent.h"
#include "AnimationComponent.h"
#include "EnemyComponent.h"
#include "CharacterComponent.h"

#include "DigDugSceneManager.h"

FireComponent::FireComponent(dae::GameObject* pParentObject)
	: Component{ pParentObject }
{
}

void FireComponent::Update(float /*deltaTime*/)
{
	CheckCollision();
}

void FireComponent::SetActive(bool isActive, EnemyComponent* pEnemy)
{
	GetGameObject()->SetIsActive(isActive);
	GetGameObject()->SetIsHidden(!isActive);

	if (isActive)
	{
		const bool isLookingLeft{ pEnemy->GetIsLookingLeft() };

		const float ownWidth{ static_cast<float>(m_pRenderComponent->GetTextureSize().x) };
		const float enemyWidth{ static_cast<float>(pEnemy->GetAnimationComponent()->GetTextureSize().x) };

		const float xPos{ ownWidth / 2.f + enemyWidth / 2.f };
		const float direction = (isLookingLeft) ? -1.f : 1.f;

		// Set position
		GetGameObject()->SetLocalPosition(xPos * direction, 0.f, 0.f);
		if (isLookingLeft) m_pRenderComponent->SetFlip(true);
		else			   m_pRenderComponent->SetFlip(false);
	}
}

void FireComponent::CheckCollision()
{
	const utils::Rect ownRect{ m_pRenderComponent->GetBoundingRect() };
	utils::Rect playerRect{};
	
	// Loop through players
	for (const auto& currentPlayer : digdug::DigDugSceneManager::GetInstance().GetCharacters())
	{
		playerRect = currentPlayer->GetAnimationComponent()->GetBoundingRect();

		// Check if overlapping
		if (utils::RectOverlaps(ownRect, playerRect))
		{
			// Set player to hit
			currentPlayer->SetHit();
		}
	}
}