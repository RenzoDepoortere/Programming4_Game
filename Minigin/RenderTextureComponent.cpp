#include "RenderTextureComponent.h"
#include "Texture2D.h"
#include "GameObject.h"
#include "Renderer.h"

dae::RenderTextureComponent::RenderTextureComponent(dae::GameObject* pParentObject)
	: Component{ pParentObject }
{
}

void dae::RenderTextureComponent::Render() const
{
	if (m_pTexture2D == nullptr) return;
	if (m_ManualRender) return;

	glm::vec3 pos{ GetGameObject()->GetWorldPosition() };
	const float angle{ GetGameObject()->GetRotation() };

	if (m_CenterTexture)
	{
		pos.x -= m_TextureSize.x / 2.f;
		pos.y -= m_TextureSize.y / 2.f;
	}

	utils::Rect destRect{};
	destRect.x = pos.x;
	destRect.y = pos.y;

	Renderer::GetInstance().RenderTexture(*m_pTexture2D, destRect, {}, angle, m_Flip);
}
void dae::RenderTextureComponent::RenderManually(const glm::vec2& pos, const utils::Rect& srcRect) const
{
	if (m_pTexture2D == nullptr) return;
	if (m_ManualRender == false) return;

	utils::Rect editedDestRect{};
	editedDestRect.x = pos.x;
	editedDestRect.y = pos.y;

	if (m_CenterTexture)
	{
		editedDestRect.x -= srcRect.width / 2.f;
		editedDestRect.y -= srcRect.height / 2.f;
	}

	// We want the rendered texture to be as big as the given srcWidth and srcHeight
	editedDestRect.width = srcRect.width;
	editedDestRect.height = srcRect.height;

	const float angle{ GetGameObject()->GetRotation() };
	Renderer::GetInstance().RenderTexture(*m_pTexture2D, editedDestRect, srcRect, angle, m_Flip);
}

void dae::RenderTextureComponent::SetTexture(std::shared_ptr<Texture2D> pTexture)
{
	m_pTexture2D = pTexture;
	m_TextureSize = m_pTexture2D->GetSize();
}