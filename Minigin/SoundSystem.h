#pragma once
#include <string>

namespace dae
{
	// Actual class
	// ************
	class SoundSystem
	{
	public:
		// Rule of five
		SoundSystem() = default;
		virtual ~SoundSystem() = default;

		SoundSystem(const SoundSystem& rhs) = delete;
		SoundSystem(SoundSystem&& rhs) = delete;
		SoundSystem& operator=(const SoundSystem& rhs) = delete;	
		SoundSystem& operator=(SoundSystem&& rhs) = delete;

		// Funtionality
		virtual void PlayAudio(unsigned int ID, int volume, int loops = -1, int channel = 0) = 0;
		
		// Warning: This can be a blocking call since it has to wait until the thread is done processing all the audioFiles
		virtual bool IsPlayingAudio(unsigned int ID, int channel = 0) = 0;
		virtual void PauseAudio(unsigned int ID, int channel = 0) = 0;
		// Warning: This can be a blocking call since it has to wait until the thread is done processing all the audioFiles
		virtual bool IsPausedAudio(unsigned int ID, int channel = 0) = 0;
		virtual void ResumeAudio(unsigned int ID, int channel = 0) = 0;
		virtual void SetVolumeAudio(unsigned int ID, int volume) = 0;

		virtual void MuteAudio(bool setMute) = 0;

		virtual void SetID(unsigned int ID, const std::string& resourceName) = 0;
	};

	// Default implementation
	// **********************
	class nullSoundSystem final : public SoundSystem
	{
	public:
		virtual ~nullSoundSystem() override = default;

		virtual void PlayAudio(unsigned int, int, int, int) override {};

		virtual bool IsPlayingAudio(unsigned int, int) override { return false; };
		virtual void PauseAudio(unsigned int, int) override {};
		virtual bool IsPausedAudio(unsigned int, int) override { return false; };
		virtual void ResumeAudio(unsigned int, int) override {};
		virtual void SetVolumeAudio(unsigned int, int) override {};

		virtual void MuteAudio(bool) override {};

		virtual void SetID(unsigned int, const std::string&) override {};
	};
}