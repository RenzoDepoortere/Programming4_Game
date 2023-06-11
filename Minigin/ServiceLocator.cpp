#include "ServiceLocator.h"

#include <iostream>

// Init
dae::nullSoundSystem dae::ServiceLocator::m_DefaultSoundSystem = dae::nullSoundSystem{};
dae::SoundSystem* dae::ServiceLocator::m_pSoundSystem = &dae::ServiceLocator::m_DefaultSoundSystem;

// Destroy
void dae::ServiceLocator::Shutdown()
{
	if (m_pSoundSystem && m_pSoundSystem != &m_DefaultSoundSystem)
	{
		delete m_pSoundSystem;
		m_pSoundSystem = &m_DefaultSoundSystem;
	}
}

// Functionality
void dae::ServiceLocator::RegisterSoundSystem(SoundSystem* pSoundSystem)
{ 
	std::cout << "Registering SoundSystem" << std::endl;

	// Delete previous soundSystem (didn't copy "Shutdown" since we will probably add more systems to this class and we don't want to delete them all)
	if (m_pSoundSystem && m_pSoundSystem != &m_DefaultSoundSystem)
	{
		delete m_pSoundSystem;
	}

	// Assign new one
	m_pSoundSystem = (pSoundSystem == nullptr) ? &m_DefaultSoundSystem : pSoundSystem;
}