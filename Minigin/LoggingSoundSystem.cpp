#include "LoggingSoundSystem.h"

#include "../DigDug/EventsEnum.h"
#include "EventManager.h"

#include <iostream>

dae::LoggingSoundSystem::LoggingSoundSystem(dae::SoundSystem* pRealSystem)
	: m_pRealSystem{ pRealSystem }
{
}

dae::LoggingSoundSystem::~LoggingSoundSystem()
{
	delete m_pRealSystem;
	m_pRealSystem = nullptr;
}

#pragma region AudioFunctionality
void dae::LoggingSoundSystem::Play(unsigned int ID, int volume, int loops)
{
	m_pRealSystem->Play(ID, volume, loops);
	std::cout << "Sound ID: " << ID << " Played at Sound Volume: " << volume << std::endl;
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
	std::cout << "Sound ID: " << ID << " set Sound Volume to: " << volume << std::endl;
}
#pragma endregion

void dae::LoggingSoundSystem::SetID(unsigned int ID, const std::string& resourceName)
{
	m_pRealSystem->SetID(ID, resourceName);
	std::cout << "Set Sound ID: " << ID << " to ResourceName: " << resourceName << std::endl;
}

void dae::LoggingSoundSystem::HandleEvent(unsigned int ID, int volume, int loops)
{
	m_pRealSystem->HandleEvent(ID, volume, loops);
	std::cout << "Sound played by event: " << ID << std::endl;
}

void dae::LoggingSoundSystem::OnSubjectDestroy()
{
	m_pRealSystem->OnSubjectDestroy();
	std::cout << "SoundSystem's Subject destroyed" << std::endl;
}