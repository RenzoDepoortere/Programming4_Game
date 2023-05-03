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

	glm::vec3 pos = GetGameObject()->GetWorldPosition();
	if (m_CenterTexture)
	{
		pos.x -= m_TextureSize.x / 2.f;
		pos.y -= m_TextureSize.y / 2.f;
	}

	Renderer::GetInstance().RenderTexture(*m_pTexture2D, pos.x, pos.y);
}

void dae::RenderTextureComponent::SetTexture(std::shared_ptr<Texture2D> pTexture)
{
	m_pTexture2D = pTexture;
	m_TextureSize = m_pTexture2D->GetSize();
}