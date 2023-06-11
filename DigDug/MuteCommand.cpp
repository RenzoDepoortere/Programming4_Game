#include "MuteCommand.h"
#include "ServiceLocator.h"

void MuteCommand::Execute(float /*deltaTime*/)
{
	m_IsMuted = !m_IsMuted;
	dae::ServiceLocator::GetSoundSystem().MuteAudio(m_IsMuted);
}