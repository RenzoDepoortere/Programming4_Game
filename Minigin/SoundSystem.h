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
		virtual void PlayAudio(unsigned int ID, int volume, int loops = -1) = 0;
		
		// Warning: This can be a blocking call since it has to wait until the thread is done processing all the audioFiles
		virtual bool IsPlayingAudio(unsigned int ID) = 0;
		virtual void PauseAudio(unsigned int ID) = 0;
		// Warning: This can be a blocking call since it has to wait until the thread is done processing all the audioFiles
		virtual bool IsPausedAudio(unsigned int ID) = 0;
		virtual void ResumeAudio(unsigned int ID) = 0;
		virtual void SetVolumeAudio(unsigned int ID, int volume) = 0;

		virtual void SetID(unsigned int ID, const std::string& resourceName) = 0;
	};

	// Default implementation
	// **********************
	class nullSoundSystem final : public SoundSystem
	{
	public:
		~nullSoundSystem() override = default;

		void PlayAudio(unsigned int, int, int) override {};
		bool IsPlayingAudio(unsigned int) override { return false; };
		void PauseAudio(unsigned int) override {};
		bool IsPausedAudio(unsigned int) override {return false; };
		void ResumeAudio(unsigned int) override {};
		void SetVolumeAudio(unsigned int, int) override {};

		void SetID(unsigned int, const std::string&) override {};
	};
}