#pragma once
#include "SoundSystem.h"
#include <memory>
#include <map>
#include <string>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>

namespace dae
{
	class AudioFile;

	class SDLSoundSystem final : public SoundSystem
	{
	public:
		// Rule of five
		SDLSoundSystem();
		~SDLSoundSystem();

		SDLSoundSystem(const SDLSoundSystem& rhs) = delete;
		SDLSoundSystem(SDLSoundSystem&& rhs) = delete;
		SDLSoundSystem& operator= (const SDLSoundSystem& rhs) = delete;
		SDLSoundSystem& operator= (SDLSoundSystem&& rhs) = delete;

		// Functionality
		void PlayAudio(unsigned int ID, int volume, int loops = -1) override;
		bool IsPlayingAudio(unsigned int ID) override;
		void PauseAudio(unsigned int ID) override;
		bool IsPausedAudio(unsigned int ID) override;
		void ResumeAudio(unsigned int ID) override;
		void SetVolumeAudio(unsigned int ID, int volume) override;

		void SetID(unsigned int ID, const std::string& resourceName) override;

	private:
		// Member variables
		// ----------------
		std::map<unsigned int, std::shared_ptr<AudioFile>> m_AudioFiles{};

		enum ThreadInstruction
		{
			LoadSFX, 
			PlaySFX,
			PauseSFX,
			ResumeSFX,
			SetVolumeSFX
		};

		struct AudioInfo
		{
			unsigned int soundID{};
			std::string resourceName{};
			ThreadInstruction threadInstruction{};

			int volume{};
			int loops{};
		};
		std::queue<AudioInfo> m_AudioQueue{};

		std::condition_variable m_ConditionVariable{};
		std::mutex m_Mutex{};
		std::jthread m_AudioThread;

		bool m_IsHandlingAudio{ false };
		bool m_IsBeingDestroyed{ false };

		// Member functions
		// ----------------
		bool IsValid(unsigned int ID, bool printError = true);
		
		void Load(unsigned int ID, const std::string& resourceName);
		void Play(unsigned int ID, int volume, int loops);
		void Pause(unsigned int ID);
		void Resume(unsigned int ID);
		void SetVolume(unsigned int ID, int volume);

		void AudioThread();
	};
}