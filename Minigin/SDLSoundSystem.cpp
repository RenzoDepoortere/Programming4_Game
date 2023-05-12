#include "SDLSoundSystem.h"
#include "SDL_mixer.h"
#include "AudioFile.h"
#include "../DigDug/EventsEnum.h"

#include "ResourceManager.h"
#include "EventManager.h"

#include <iostream>

dae::SDLSoundSystem::SDLSoundSystem()
{
#ifndef _DEBUG
	// Subcsribe to event
	dae::EventManager<unsigned int, int, int>::GetInstance().Subscribe(event::PauseMenu, this);
#endif // _DEBUG

	// Start thread
	m_AudioThread = std::jthread{&dae::SDLSoundSystem::AudioThread, this };
}
dae::SDLSoundSystem::~SDLSoundSystem()
{
	m_IsBeingDestroyed = true;
	m_ConditionVariable.notify_all();
}

#pragma region AudioFunctionality
void dae::SDLSoundSystem::Play(unsigned int ID, int volume, int loops)
{
	// Get audioClip, load if didn't load yet
	if (IsValid(ID, false, false) == false && m_IDs.contains(ID))
	{
		m_AudioFiles[ID] = ResourceManager::GetInstance().LoadSound(GetResourceName(ID));
	}
	else if(m_IDs.contains(ID) == false)
	{
		std::cout << "Error: tried to load non-existent soundID" << std::endl;
		return;
	}


	// Set volume and play
	m_AudioFiles[ID]->SetVolume(volume);
	m_AudioFiles[ID]->Play(loops);
}
bool dae::SDLSoundSystem::IsPlaying(unsigned int ID)
{
	if (IsValid(ID)) return m_AudioFiles[ID]->IsPlaying();
	else			 return false;
}
void dae::SDLSoundSystem::Pause(unsigned int ID)
{
	if (IsValid(ID)) m_AudioFiles[ID]->Pause();
}
bool dae::SDLSoundSystem::IsPaused(unsigned int ID)
{
	if (IsValid(ID)) return m_AudioFiles[ID]->IsPaused();
	else			 return false;
}
void dae::SDLSoundSystem::Resume(unsigned int ID)
{
	if (IsValid(ID)) m_AudioFiles[ID]->Resume();
}
void dae::SDLSoundSystem::SetVolume(unsigned int ID, int volume)
{
	if (IsValid(ID)) m_AudioFiles[ID]->SetVolume(volume);
}
#pragma endregion

unsigned int dae::SDLSoundSystem::SetID(const std::string& resourceName)
{
	m_IDs[m_NextFreeID] = resourceName;
	return m_NextFreeID++;
}

void dae::SDLSoundSystem::HandleEvent(int /*eventID*/, unsigned int soundID, int volume, int loops)
{
	// Lock mutex
	{
		std::lock_guard<std::mutex> lockGuard{ m_Mutex };

		// Store info
		m_AudioInfo.soundID = soundID;
		m_AudioInfo.volume = volume;
		m_AudioInfo.loops = loops;
	}

	// Notify
	m_ConditionVariable.notify_all();
}

bool dae::SDLSoundSystem::IsValid(unsigned int ID, bool checkIsInIDs, bool printError)
{
	auto iterator{ m_AudioFiles.find(ID) };
	bool inIDs{ true };
	if (checkIsInIDs) inIDs = m_IDs.contains(ID);

	if (iterator == m_AudioFiles.end() && inIDs)
	{
		if (printError) std::cout << "Error: tried to load non-existent soundID" << std::endl;
		return false;
	}
	else return true;
}

void dae::SDLSoundSystem::AudioThread()
{
	// Unique lock mutex
	std::unique_lock<std::mutex> uniqueLock{ m_Mutex };

	// Keep looping, till end of program
	while (m_IsBeingDestroyed == false)
	{
		// Wait for notification
		m_ConditionVariable.wait(uniqueLock);
		
		// If is not being destroyed
		if (m_IsBeingDestroyed == false)
		{
			// Use info to play audio
			Play(m_AudioInfo.soundID, m_AudioInfo.volume, m_AudioInfo.loops);
		}
	}
}