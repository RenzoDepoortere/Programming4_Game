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
void dae::LoggingSoundSystem::PlayAudio(unsigned int ID, int volume, int loops, int channel)
{
	m_pRealSystem->PlayAudio(ID, volume, loops, channel);
	std::cout << "Sound ID: " << ID << " Played at Sound Volume: " << volume << std::endl;
}
bool dae::LoggingSoundSystem::IsPlayingAudio(unsigned int ID, int channel)
{
	const bool isPlaying{ m_pRealSystem->IsPlayingAudio(ID, channel) };
	std::cout << "Sound ID: " << ID << ", IsPlaying: " << isPlaying << std::endl;

	return isPlaying;
}
void dae::LoggingSoundSystem::PauseAudio(unsigned int ID, int channel)
{
	m_pRealSystem->PauseAudio(ID, channel);
	std::cout << "Sound ID: " << ID << ", Paused" << std::endl;
}
bool dae::LoggingSoundSystem::IsPausedAudio(unsigned int ID, int channel)
{
	const bool isPaused{ m_pRealSystem->IsPausedAudio(ID, channel) };
	std::cout << "Sound ID: " << ID << ", IsPaused: " << isPaused << std::endl;

	return isPaused;
}
void dae::LoggingSoundSystem::ResumeAudio(unsigned int ID, int channel)
{
	m_pRealSystem->ResumeAudio(ID, channel);
	std::cout << "Sound ID: " << ID << ", Resumed" << std::endl;
}
void dae::LoggingSoundSystem::SetVolumeAudio(unsigned int ID, int volume)
{
	m_pRealSystem->SetVolumeAudio(ID, volume);
	std::cout << "Sound ID: " << ID << " set Sound Volume to: " << volume << std::endl;
}

void dae::LoggingSoundSystem::MuteAudio(bool setMute)
{
	m_pRealSystem->MuteAudio(setMute);
	std::cout << "Sound muted: " << setMute << std::endl;
}
#pragma endregion

void dae::LoggingSoundSystem::SetID(unsigned int ID, const std::string& resourceName)
{
	m_pRealSystem->SetID(ID, resourceName);
	std::cout << "Set Sound ID: " << ID << " to ResourceName: " << resourceName << std::endl;
}