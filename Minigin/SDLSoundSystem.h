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
		void PlayAudio(unsigned int ID, int volume, int loops = -1, int channel = 0) override;

		bool IsPlayingAudio(unsigned int ID, int channel = 0) override;
		void PauseAudio(unsigned int ID, int channel = 0) override;
		bool IsPausedAudio(unsigned int ID, int channel = 0) override;
		void ResumeAudio(unsigned int ID, int channel = 0) override;
		void SetVolumeAudio(unsigned int ID, int volume) override;

		virtual void MuteAudio(bool setMute) override;

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
			int channel{};
		};
		std::queue<AudioInfo> m_AudioQueue{};

		std::condition_variable m_ConditionVariable{};
		std::mutex m_Mutex{};
		std::jthread m_AudioThread;

		bool m_IsHandlingAudio{ false };
		bool m_IsBeingDestroyed{ false };

		bool m_IsMuted{ false };

		// Member functions
		// ----------------
		bool IsValid(unsigned int ID, bool printError = true);
		
		void Load(unsigned int ID, const std::string& resourceName);
		void Play(unsigned int ID, int volume, int loops, int channel);
		void Pause(unsigned int ID, int channel);
		void Resume(unsigned int ID, int channel);
		void SetVolume(unsigned int ID, int volume);

		void AudioThread();
	};
}