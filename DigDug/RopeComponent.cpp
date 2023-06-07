#include "RopeComponent.h"

#include "RenderTextureComponent.h"
#include "GameObject.h"

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
}
void RopeComponent::Render() const
{
	const glm::vec3 pos{ GetGameObject()->GetWorldPosition() };
	m_pRenderTextureComponent->RenderManually(glm::vec2{ pos.x, pos.y }, m_SrcRect);
}

void RopeComponent::StartThrow()
{
	// Set throwing to true
	m_IsThrowing = true;

	// Reset visuals
	m_SrcRect.width = 0.f;
	m_TextureFill = 0.f;
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