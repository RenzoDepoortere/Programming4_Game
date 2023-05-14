#pragma once
#include "Observer.h"
#include <string>

namespace dae
{
	// Actual class
	// ************
	class SoundSystem : public Observer<int, int>
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
		virtual void Play(unsigned int ID, int volume, int loops = -1) = 0;
		virtual bool IsPlaying(unsigned int ID) = 0;
		virtual void Pause(unsigned int ID) = 0;
		virtual bool IsPaused(unsigned int ID) = 0;
		virtual void Resume(unsigned int ID) = 0;
		virtual void SetVolume(unsigned int ID, int volume) = 0;

		virtual void SetID(unsigned int ID, const std::string& resourceName) = 0;

		virtual void HandleEvent(unsigned int ID, int volume, int loops = 1) = 0;
		virtual void OnSubjectDestroy() = 0;
	};

	// Default implementation
	// **********************
	class nullSoundSystem final : public SoundSystem
	{
	public:
		~nullSoundSystem() override = default;

		void Play(unsigned int, int, int) override {};
		bool IsPlaying(unsigned int) override { return false; };
		void Pause(unsigned int) override {};
		bool IsPaused(unsigned int) override {return false; };
		void Resume(unsigned int) override {};
		void SetVolume(unsigned int, int) override {};

		void SetID(unsigned int, const std::string&) override {};

		void HandleEvent(unsigned int, int, int) override {};
		void OnSubjectDestroy() override {};
	};
}