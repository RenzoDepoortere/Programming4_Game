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
		void Play(int loops = -1);
		bool IsPlaying();
		void Pause();
		bool IsPaused();
		void Resume();
		void SetVolume(int volume);

	private:
		// Member variables
		Mix_Chunk* m_pChunk{ nullptr };
	};
}