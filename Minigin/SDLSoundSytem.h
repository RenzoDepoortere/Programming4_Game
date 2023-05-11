#pragma once
#include "SoundSystem.h"

namespace dae
{
	class SDLSoundSytem final : SoundSystem
	{
	public:
		// Rule of five
		~SDLSoundSytem() = default;

		SDLSoundSytem(const SDLSoundSytem& rhs) = delete;
		SDLSoundSytem(SDLSoundSytem&& rhs) = delete;
		SDLSoundSytem& operator= (const SDLSoundSytem& rhs) = delete;
		SDLSoundSytem& operator= (SDLSoundSytem&& rhs) = delete;

		// Functionality
		void Play(unsigned int ID, float volume) override;

	private:
	};
}