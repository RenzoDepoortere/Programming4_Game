#include "LoggingSoundSystem.h"

LoggingSoundSystem::~LoggingSoundSystem()
{
	delete m_pRealSystem;
	m_pRealSystem = nullptr;
}

void LoggingSoundSystem::Play(unsigned int ID, float volume)
{

}