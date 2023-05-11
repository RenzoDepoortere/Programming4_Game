#include "ServiceLocator.h"

// Init
void dae::ServiceLocator::Initialize()
{
	m_pSoundSystem = &m_DefaultSoundSystem;
}

// Destroy
void dae::ServiceLocator::Shutdown()
{
	if (m_pSoundSystem && m_pSoundSystem != &m_DefaultSoundSystem)
	{
		delete m_pSoundSystem;
		m_pSoundSystem = &m_DefaultSoundSystem;
	}
}