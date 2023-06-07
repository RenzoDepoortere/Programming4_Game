#include "AnimationComponent.h"

#include "GameObject.h"

dae::AnimationComponent::AnimationComponent(dae::GameObject* pParentObject)
	: dae::RenderTextureComponent{ pParentObject }
{
	// Render manually
	SetManualRender(true);

	// Temp
	m_SrcRect.width = 25.f;
	m_SrcRect.height = 25.f;
}

void dae::AnimationComponent::Render() const
{
	// Get data
	glm::vec3 pos{ GetGameObject()->GetWorldPosition() };
	const glm::ivec2 textureSize{ GetTextureSize() };

	// Render
	RenderManually(glm::vec2{ pos.x, pos.y }, m_SrcRect);
}