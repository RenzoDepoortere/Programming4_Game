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
		void Play(int loops = -1, int channel = 0);
		bool IsPlaying(int channel = 0);
		void Pause(int channel = 0);
		bool IsPaused(int channel = 0);
		void Resume(int channel = 0);
		void SetVolume(int volume);

	private:
		// Member variables
		void* m_pChunk{ nullptr };
	};
}