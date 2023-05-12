#include "LoggingSoundSystem.h"
#include <iostream>

dae::LoggingSoundSystem::~LoggingSoundSystem()
{
	delete m_pRealSystem;
	m_pRealSystem = nullptr;
}

void dae::LoggingSoundSystem::Play(unsigned int ID, int volume, int loops)
{
	m_pRealSystem->Play(ID, volume, loops);
	std::cout << "Sound ID: " << ID << ", Played, Sound Volume: " << volume << std::endl;
}
bool dae::LoggingSoundSystem::IsPlaying(unsigned int ID)
{
	const bool isPlaying{ m_pRealSystem->IsPlaying(ID) };
	std::cout << "Sound ID: " << ID << ", IsPlaying: " << isPlaying << std::endl;
	
	return isPlaying;
}
void dae::LoggingSoundSystem::Pause(unsigned int ID)
{
	m_pRealSystem->Pause(ID);
	std::cout << "Sound ID: " << ID << ", Paused" << std::endl;
}
bool dae::LoggingSoundSystem::IsPaused(unsigned int ID)
{
	const bool isPaused{ m_pRealSystem->IsPaused(ID) };
	std::cout << "Sound ID: " << ID << ", IsPaused: " << isPaused << std::endl;

	return isPaused;
}
void dae::LoggingSoundSystem::Resume(unsigned int ID)
{
	m_pRealSystem->Resume(ID);
	std::cout << "Sound ID: " << ID << ", Resumed" << std::endl;
}
void dae::LoggingSoundSystem::SetVolume(unsigned int ID, int volume)
{
	m_pRealSystem->SetVolume(ID, volume);
	std::cout << "Sound ID: " << ID << ", Sound Volume: " << volume << std::endl;
}