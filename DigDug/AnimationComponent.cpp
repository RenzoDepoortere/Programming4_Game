#include "AnimationComponent.h"

#include "../Minigin/GameObject.h"

dae::AnimationComponent::AnimationComponent(dae::GameObject* pParentObject)
	: dae::RenderTextureComponent{ pParentObject }
{
	// Render manually
	SetManualRender(true);
}

void dae::AnimationComponent::Update(float deltaTime)
{
	// Don't update on pause
	if (m_IsPaused) return;

	const float frameTime{ 1.f / m_FramesPerSecond };

	// Count
	m_TimePassed += deltaTime;
	if (frameTime <= m_TimePassed)
	{
		m_TimePassed -= frameTime;

		// Go to next frame
		m_CurrentFrame = (m_CurrentFrame + 1) % m_MaxFrames;
		m_SrcRect.x = m_CurrentFrame * m_SrcRect.width;

		// If restarted, set bool
		if (m_PlayedOnce == false && m_CurrentFrame == 0) m_PlayedOnce = true;
	}
}

void dae::AnimationComponent::Render() const
{
	// Return if no texture
	if (GetTexture() == nullptr) return;

	// Get data
	glm::vec3 pos{ GetGameObject()->GetWorldPosition() };
	const glm::ivec2 textureSize{ GetTextureSize() };

	// Render
	RenderManually(glm::vec2{ pos.x, pos.y }, m_SrcRect);
}

void dae::AnimationComponent::SetSingleSpriteSize(float spriteSize)
{
	m_SrcRect.width = spriteSize;
	m_SrcRect.height = spriteSize;
}

void dae::AnimationComponent::SetTexture(std::shared_ptr<Texture2D> pTexture)
{
	// Set texture
	RenderTextureComponent::SetTexture(pTexture);

	// Reset animation
	m_TimePassed = 0.f;
	m_CurrentFrame = 0;

	m_PlayedOnce = false;
}