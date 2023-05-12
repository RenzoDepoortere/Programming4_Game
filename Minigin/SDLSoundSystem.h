#pragma once
#include "SoundSystem.h"
#include <memory>
#include <map>
#include <string>
#include <mutex>
#include <condition_variable>
#include <thread>

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
		void Play(unsigned int ID, int volume, int loops = -1) override;
		bool IsPlaying(unsigned int ID) override;
		void Pause(unsigned int ID) override;
		bool IsPaused(unsigned int ID) override;
		void Resume(unsigned int ID) override;
		void SetVolume(unsigned int ID, int volume) override;

		unsigned int SetID(const std::string& resourceName) override;

		void HandleEvent(int eventID, unsigned int soundID, int volume, int loops = 1) override;
		void OnSubjectDestroy() override {};

	private:
		// Member variables
		// ----------------
		std::map<unsigned int, std::shared_ptr<AudioFile>> m_AudioFiles{};

		// Todo: fix, this could probably be done in a better way
		unsigned int m_NextFreeID{ 0 };
		std::map<unsigned int, std::string> m_IDs{};

		struct audioInfo
		{
			unsigned int soundID{};
			int volume{};
			int loops{};
		};
		audioInfo m_AudioInfo{};

		std::condition_variable m_ConditionVariable{};
		std::mutex m_Mutex{};
		std::jthread m_AudioThread;

		bool m_IsBeingDestroyed{ false };

		// Member functions
		// ----------------
		bool IsValid(unsigned int ID, bool checkIsInIDs = false, bool printError = true);
		std::string GetResourceName(unsigned int ID) { return m_IDs[ID]; }

		void AudioThread();
	};
}