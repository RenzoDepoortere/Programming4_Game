#pragma once

struct Mix_Chunk;
namespace dae
{
	class AudioFile final
	{
	public:
		// Rule of five
		explicit AudioFile(Mix_Chunk* pChunk);
		~AudioFile();

		AudioFile(const AudioFile& rhs) = delete;
		AudioFile(AudioFile&& rhs) = delete;
		AudioFile& operator= (const AudioFile& rhs) = delete;
		AudioFile& operator= (AudioFile&& rhs) = delete;

		// Functionality


	private:
		// Member variables
		Mix_Chunk* m_pChunk{ nullptr };
	};
}