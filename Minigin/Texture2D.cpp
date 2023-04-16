#include <SDL.h>
#include "Texture2D.h"
#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"

dae::Texture2D::Texture2D(SDL_Texture* pTexture)
	: m_Texture{ pTexture }
{
	
}

dae::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_Texture);
	m_Texture = nullptr;
}

glm::ivec2 dae::Texture2D::GetSize() const
{
	SDL_Rect dst;
	SDL_QueryTexture(GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);

	return { dst.w,dst.h };
}
SDL_Texture* dae::Texture2D::GetSDLTexture() const
{
	return m_Texture;
}
void dae::Texture2D::SetSDLTexture(SDL_Texture* pTexture)
{
	m_Texture = pTexture;
}
