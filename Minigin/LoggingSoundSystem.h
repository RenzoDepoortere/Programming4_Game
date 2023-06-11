#pragma once
#include "SoundSystem.h"

namespace dae
{
	class LoggingSoundSystem final : public SoundSystem
	{
	public:
		// Destructor
		explicit LoggingSoundSystem(SoundSystem* pRealSystem);
		virtual ~LoggingSoundSystem() override;

		// Functionality
		virtual void PlayAudio(unsigned int ID, int volume, int loops = -1, int channel = 0) override;
		virtual bool IsPlayingAudio(unsigned int ID, int channel = 0) override;
		virtual void PauseAudio(unsigned int ID, int channel = 0) override;
		virtual bool IsPausedAudio(unsigned int ID, int channel = 0) override;
		virtual void ResumeAudio(unsigned int ID, int channel = 0) override;
		virtual void SetVolumeAudio(unsigned int ID, int volume) override;

		virtual void MuteAudio(bool setMute) override;

		virtual void SetID(unsigned int ID, const std::string& resourceName) override;

	private:
		// Member variables
		SoundSystem* m_pRealSystem;
	};
}