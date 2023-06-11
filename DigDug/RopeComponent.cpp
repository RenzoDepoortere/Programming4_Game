#include "RopeComponent.h"

#include "RenderTextureComponent.h"
#include "GameObject.h"
#include "CharacterComponent.h"
#include "GridHelpers.h"
#include "CharacterComponent.h"
#include "EnemyManager.h"
#include "EnemyComponent.h"
#include "AnimationComponent.h"

#include "DigDugSceneManager.h"

#include "Renderer.h"

#include <SDL.h>

RopeComponent::RopeComponent(dae::GameObject* pParentObject)
	: Component{ pParentObject }
{
}

void RopeComponent::Update(float deltaTime)
{
	// Return if not throwing
	if (m_IsThrowing == false) return;

	// Update
	HandleRopeExtension(deltaTime);
	CheckCollision();
}
void RopeComponent::Render() const
{
	// Render rope
	const glm::vec3 pos{ GetGameObject()->GetWorldPosition() };
	m_pRenderTextureComponent->RenderManually(glm::vec2{ pos.x, pos.y }, m_SrcRect);
}

void RopeComponent::StartThrow(CharacterComponent* pPlayer)
{
	// Reset bools
	m_IsThrowing = true;
	m_CaughtEnemy = false;

	// Reset visuals
	m_SrcRect.width = 0.f;
	m_TextureFill = 0.f;

	// Set movementDirection
	switch (pPlayer->GetLookingDirection())
	{
	case player::Up:
		m_MovementDirection = glm::vec2{ 0, -1 };
		break;

	case player::Down:
		m_MovementDirection = glm::vec2{ 0, 1 };
		break;

	case player::Left:
		m_MovementDirection = glm::vec2{ -1, 0 };
		break;

	case player::Right:
		m_MovementDirection = glm::vec2{ 1, 0 };
		break;
	}

	// Set startPos
	m_StartPosition = pPlayer->GetGameObject()->GetWorldPosition();
	m_pPlayer = pPlayer;
}

void RopeComponent::SetRenderTextureComponent(dae::RenderTextureComponent* pRenderTextureComponent)
{
	// Set textureComponent
	m_pRenderTextureComponent = pRenderTextureComponent;
	m_pRenderTextureComponent->SetManualRender(true);

	// Set srcRect
	const glm::ivec2 textureSize{ m_pRenderTextureComponent->GetTextureSize() };

	m_TextureWidth = static_cast<int>(textureSize.x);
	m_SrcRect.height = static_cast<float>(textureSize.y);
}

void RopeComponent::HandleRopeExtension(float deltaTime)
{
	// Fill rope
	m_TextureFill += (m_ThrowSpeed / 100.f) * deltaTime;
	if (1.f <= m_TextureFill) m_TextureFill = 1.f;

	m_SrcRect.width = m_TextureFill * m_TextureWidth;

	// If rope reached end, done throwing
	if (m_TextureFill == 1.f)
	{
		m_IsThrowing = false;
	}
}
void RopeComponent::CheckCollision()
{
	// Get outerPoint pos
	glm::vec3 pointPos{ m_StartPosition };
	if (m_MovementDirection.x != 0) pointPos.x += m_TextureWidth / 2.f * (m_MovementDirection.x + m_TextureFill);
	else  							pointPos.y += m_TextureWidth / 2.f * (m_MovementDirection.y + m_TextureFill);

	// Check if hit dirtCell
	if (grid::IsInDirtCell(pointPos, m_pGrid))
	{
		m_IsThrowing = false;
		return;
	}

	// Check if hit enemy
	utils::Rect ownRect{ m_pRenderTextureComponent->GetBoundingRect() };
	ownRect.width *= m_TextureFill;
	
	utils::Rect enemyRect{};

	for (const auto& currentEnemy : digdug::DigDugSceneManager::GetInstance().GetEnemies())
	{
		enemyRect = currentEnemy->GetAnimationComponent()->GetBoundingRect();

		if (utils::RectOverlaps(ownRect, enemyRect))
		{
			// Caught enemy
			m_CaughtEnemy = true;
			m_IsThrowing = false;

			// Couple enemy to player
			m_pPlayer->SetCaughtEnemy(currentEnemy);
			currentEnemy->SetCaught(true);
		}
	}
}