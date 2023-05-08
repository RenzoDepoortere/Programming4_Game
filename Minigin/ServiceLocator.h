#pragma once
#include "SoundSystem.h"

namespace dae
{
	class ServiceLocator final
	{
	public:
		// Sound
		static SoundSystem& GetSoundSystem() { return *m_pSoundSystem; }
		static void RegisterSoundSystem(SoundSystem* pSoundSystem) { m_pSoundSystem = pSoundSystem; }

	private:
		// Member variables
		static SoundSystem* m_pSoundSystem;
	};
}