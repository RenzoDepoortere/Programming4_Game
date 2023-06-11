#pragma once
#include <string>
#include <memory>
#include <fstream>
#include <map>

#include "Singleton.h"

namespace dae
{
	class Font;
	class Texture2D;
	class AudioFile;

	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init(const std::string& data);
		std::shared_ptr<dae::Texture2D> LoadTexture(const std::string& file);
		std::shared_ptr<Font> LoadFont(const std::string& file, unsigned int size) const;
		std::shared_ptr<std::ifstream> LoadReadFile(const std::string& file) const;
		std::shared_ptr<std::ofstream> LoadWriteFile(const std::string& file) const;
		std::shared_ptr<AudioFile> LoadSound(const std::string& file);

	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;

		// Variables
		std::string m_dataPath;
		std::map<std::string, std::shared_ptr<dae::Texture2D>> m_pTextures{};
		std::map<std::string, std::shared_ptr<dae::AudioFile>> m_pAudioFiles{};
	};
}
