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
		void PlayAudio(unsigned int ID, int volume, int loops = -1) override;
		bool IsPlayingAudio(unsigned int ID) override;
		void PauseAudio(unsigned int ID) override;
		bool IsPausedAudio(unsigned int ID) override;
		void ResumeAudio(unsigned int ID) override;
		void SetVolumeAudio(unsigned int ID, int volume) override;

		void SetID(unsigned int ID, const std::string& resourceName) override;

	private:
		// Member variables
		SoundSystem* m_pRealSystem;
	};
}