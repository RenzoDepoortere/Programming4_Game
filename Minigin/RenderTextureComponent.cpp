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

	const auto& pos = GetGameObject()->GetWorldPosition();
	Renderer::GetInstance().RenderTexture(*m_pTexture2D, pos.x, pos.y);
}

void dae::RenderTextureComponent::SetTexture(std::shared_ptr<dae::Texture2D> pTexture)
{
	m_pTexture2D = pTexture;
}