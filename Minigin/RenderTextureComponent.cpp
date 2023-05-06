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

	glm::vec3 pos = GetGameObject()->GetWorldPosition();
	if (m_CenterTexture)
	{
		pos.x -= m_TextureSize.x / 2.f;
		pos.y -= m_TextureSize.y / 2.f;
	}

	Renderer::GetInstance().RenderTexture(*m_pTexture2D, pos.x, pos.y);
}
void dae::RenderTextureComponent::RenderManually(float posX, float posY, float srcLeft, float srcTop, float srcWidth, float srcHeight) const
{
	if (m_pTexture2D == nullptr) return;
	if (m_ManualRender == false) return;

	glm::vec3 pos = { posX, posY, 0.f };
	if (m_CenterTexture)
	{
		pos.x -= srcWidth / 2.f;
		pos.y -= srcHeight / 2.f;
	}

	// We want the rendered texture to be as big as the given srcWidth and srcHeight
	Renderer::GetInstance().RenderTexture(*m_pTexture2D, pos.x, pos.y, srcWidth, srcHeight, srcLeft, srcTop, srcWidth, srcHeight);
}

void dae::RenderTextureComponent::SetTexture(std::shared_ptr<Texture2D> pTexture)
{
	m_pTexture2D = pTexture;
	m_TextureSize = m_pTexture2D->GetSize();
}