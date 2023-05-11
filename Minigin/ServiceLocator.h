#pragma once
#include "SoundSystem.h"

namespace dae
{
	class ServiceLocator final
	{
	public:
		// Destructor
		static void Initialize();
		static void Shutdown();
		
		// Sound
		static SoundSystem& GetSoundSystem() { return *m_pSoundSystem; }
		static void RegisterSoundSystem(SoundSystem* pSoundSystem) 
		{ m_pSoundSystem = (pSoundSystem == nullptr) ? &m_DefaultSoundSystem : m_pSoundSystem; }

	private:
		// Member variables
		static SoundSystem* m_pSoundSystem;
		static nullSoundSystem m_DefaultSoundSystem;
	};
}