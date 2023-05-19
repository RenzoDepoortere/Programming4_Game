#pragma once

namespace dae
{
	class AudioFile final
	{
	public:
		// Rule of five
		explicit AudioFile(void* pChunk);
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
		void* m_pChunk{ nullptr };
	};
}