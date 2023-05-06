#include <stdexcept>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "ResourceManager.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

void dae::ResourceManager::Init(const std::string& dataPath)
{
	m_dataPath = dataPath;

	if (TTF_Init() != 0)
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

std::shared_ptr<dae::Texture2D> dae::ResourceManager::LoadTexture(const std::string& file) const
{
	const auto fullPath = m_dataPath + file;
	auto texture = IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(), fullPath.c_str());
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
	}

	return std::make_shared<dae::Texture2D>(texture);
}

std::shared_ptr<dae::Font> dae::ResourceManager::LoadFont(const std::string& file, unsigned int size) const
{
	return std::make_shared<Font>(m_dataPath + file, size);
}

std::shared_ptr<std::ifstream> dae::ResourceManager::LoadFile(const std::string& file) const
{
	return std::make_shared<std::ifstream>(m_dataPath + file);
}