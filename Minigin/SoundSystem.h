#pragma once

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
		virtual void Play(unsigned int ID, float volume) = 0;
	};

	// Default implementation
	// **********************
	class nullSoundSystem final : public SoundSystem
	{
	public:
		~nullSoundSystem() override = default;
		void Play(unsigned int, float) override {};
	};
}