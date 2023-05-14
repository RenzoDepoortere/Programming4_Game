#pragma once
#include "SoundSystem.h"

namespace dae
{
	class LoggingSoundSystem final : public SoundSystem
	{
	public:
		// Destructor
		explicit LoggingSoundSystem(SoundSystem* pRealSystem);
		~LoggingSoundSystem();

		// Functionality
		void Play(unsigned int ID, int volume, int loops = -1) override;
		bool IsPlaying(unsigned int ID) override;
		void Pause(unsigned int ID) override;
		bool IsPaused(unsigned int ID) override;
		void Resume(unsigned int ID) override;
		void SetVolume(unsigned int ID, int volume) override;

		void SetID(unsigned int ID, const std::string& resourceName) override;

		void HandleEvent(unsigned int ID, int volume, int loops = 1) override;
		void OnSubjectDestroy() override;

	private:
		// Member variables
		SoundSystem* m_pRealSystem;
	};
}