#pragma once
#include <string>
#include <memory>
#include <fstream>

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
		std::shared_ptr<dae::Texture2D> LoadTexture(const std::string& file) const;
		std::shared_ptr<Font> LoadFont(const std::string& file, unsigned int size) const;
		std::shared_ptr<std::ifstream> LoadFile(const std::string& file) const;
		std::shared_ptr<AudioFile> LoadSound(const std::string& file) const;

	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		std::string m_dataPath;
	};
}
