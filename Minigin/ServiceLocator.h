#pragma once
#include "SoundSystem.h"

namespace dae
{
	class ServiceLocator final
	{
	public:
		// Destructor
		static void Shutdown();
		
		// Sound
		static SoundSystem& GetSoundSystem() { return *m_pSoundSystem; }
		static void RegisterSoundSystem(SoundSystem* pSoundSystem);

	private:
		// Member variables
		static SoundSystem* m_pSoundSystem;
		static nullSoundSystem m_DefaultSoundSystem;
	};
}