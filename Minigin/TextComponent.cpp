#include <stdexcept>
#include <SDL_ttf.h>

#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "GameObject.h"
#include "ResourceManager.h"

dae::TextComponent::TextComponent(dae::GameObject* pParentObject)
	: Component{ pParentObject }
	, m_NeedsUpdate(true)
{
	// Default variables
	m_Text = "Hello";
	m_TextColor = SDL_Color{ 255,255,255 };
	m_pTexture2D = std::make_shared<dae::Texture2D>(nullptr);
}

void dae::TextComponent::Update(float)
{
	if (m_pTexture2D == nullptr) return;
	if (m_Font == nullptr) return;

	if (m_NeedsUpdate)
	{
		const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), m_TextColor);
		if (surf == nullptr) 
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}

		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr) 
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}

		SDL_FreeSurface(surf);

		m_pTexture2D->SetSDLTexture(texture);
		m_NeedsUpdate = false;
	}
}
void dae::TextComponent::Render() const
{
	if (m_pTexture2D == nullptr) return;
	if (m_Font == nullptr) return;

	const auto& pos = GetGameObject()->GetWorldPosition();
	utils::Rect destRect{};
	destRect.x = pos.x;
	destRect.y = pos.y;

	Renderer::GetInstance().RenderTexture(*m_pTexture2D, destRect);
}

// This implementation uses the "dirty flag" pattern
void dae::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}
void dae::TextComponent::SetFont(std::shared_ptr<Font> font)
{
	m_Font = font;
	m_NeedsUpdate = true;
}
void dae::TextComponent::SetColor(const SDL_Color& color)
{
	m_TextColor = color;
	m_NeedsUpdate = true;
}
void dae::TextComponent::SetTexture(std::shared_ptr<Texture2D> pTexture)
{
	m_pTexture2D = pTexture;
	m_NeedsUpdate = true;
}
glm::ivec2 dae::TextComponent::GetTextureSize() const
{
	if (m_pTexture2D)
	{
		return m_pTexture2D->GetSize();
	}
	else
	{
		return {};
	}
}