#include "LoggingSoundSystem.h"
#include <iostream>

dae::LoggingSoundSystem::~LoggingSoundSystem()
{
	delete m_pRealSystem;
	m_pRealSystem = nullptr;
}

void dae::LoggingSoundSystem::Play(unsigned int ID, float volume)
{
	m_pRealSystem->Play(ID, volume);
	std::cout << "Sound ID: " << ID << ", Sound Volume: " << volume << std::endl;
}