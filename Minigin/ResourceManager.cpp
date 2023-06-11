#include <stdexcept>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "ResourceManager.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "AudioFile.h"

void dae::ResourceManager::Init(const std::string& dataPath)
{
	m_dataPath = dataPath;

	if (TTF_Init() != 0)
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

std::shared_ptr<dae::Texture2D> dae::ResourceManager::LoadTexture(const std::string& file)
{
	// Check if already has texture
	auto textureIt{ m_pTextures.find(file) };
	if (textureIt != m_pTextures.end()) return (*textureIt).second;

	// Load texture
	const auto fullPath = m_dataPath + file;
	auto texture = IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(), fullPath.c_str());
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
	}

	// Store and return
	m_pTextures[file] = std::make_shared<dae::Texture2D>(texture);
	return m_pTextures[file];
}

std::shared_ptr<dae::Font> dae::ResourceManager::LoadFont(const std::string& file, unsigned int size) const
{
	return std::make_shared<Font>(m_dataPath + file, size);
}

std::shared_ptr<std::ifstream> dae::ResourceManager::LoadReadFile(const std::string& file) const
{
	return std::make_shared<std::ifstream>(m_dataPath + file);
}
std::shared_ptr<std::ofstream> dae::ResourceManager::LoadWriteFile(const std::string& file) const
{
	return std::make_shared<std::ofstream>(m_dataPath + file, std::ios::app);
}

std::shared_ptr<dae::AudioFile> dae::ResourceManager::LoadSound(const std::string& file)
{
	// Check if already has sound
	auto audioIt{ m_pAudioFiles.find(file) };
	if (audioIt != m_pAudioFiles.end()) return (*audioIt).second;

	// Load sound
	const auto fullPath = m_dataPath + file;
	Mix_Chunk* pChunk{ Mix_LoadWAV(fullPath.c_str()) };
	if (pChunk == nullptr)
	{
		throw std::runtime_error(std::string("Failed to load audio: ") + SDL_GetError());
	}

	// Store and return
	m_pAudioFiles[file] = std::make_shared<dae::AudioFile>(pChunk);
	return m_pAudioFiles[file];
}

