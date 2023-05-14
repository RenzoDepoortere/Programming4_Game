#include "SDLSoundSystem.h"
#include "SDL_mixer.h"
#include "AudioFile.h"

#include "ResourceManager.h"

#include <iostream>

dae::SDLSoundSystem::SDLSoundSystem()
{
	// Start thread
	m_AudioThread = std::jthread{&dae::SDLSoundSystem::AudioThread, this };
}
dae::SDLSoundSystem::~SDLSoundSystem()
{
	m_IsBeingDestroyed = true;
	m_ConditionVariable.notify_all();
}

#pragma region AudioFunctionality
void dae::SDLSoundSystem::PlayAudio(unsigned int ID, int volume, int loops)
{
	// Lock mutex
	{
		std::lock_guard<std::mutex> lockGuard{ m_Mutex };

		// Store info
		AudioInfo info{};
		info.soundID = ID;
		info.volume = volume;
		info.loops = loops;
		info.threadInstruction = PlaySFX;

		m_AudioQueue.push(info);
	}

	// Notify
	m_ConditionVariable.notify_all();
}
bool dae::SDLSoundSystem::IsPlayingAudio(unsigned int ID)
{
	// Wait untill the other thread is done processing the audioFiles
	while (m_IsHandlingAudio)
	{
	}

	if (IsValid(ID)) return m_AudioFiles[ID]->IsPlaying();
	else			 return false;
}
void dae::SDLSoundSystem::PauseAudio(unsigned int ID)
{
	// Lock mutex
	{
		std::lock_guard<std::mutex> lockGuard{ m_Mutex };

		// Store info
		AudioInfo info{};
		info.soundID = ID;
		info.threadInstruction = PauseSFX;

		m_AudioQueue.push(info);
	}

	// Notify
	m_ConditionVariable.notify_all();
}
bool dae::SDLSoundSystem::IsPausedAudio(unsigned int ID)
{
	// Wait untill the other thread is done processing the audioFiles
	while (m_IsHandlingAudio)
	{
	}

	if (IsValid(ID)) return m_AudioFiles[ID]->IsPaused();
	else			 return false;
}
void dae::SDLSoundSystem::ResumeAudio(unsigned int ID)
{
	// Lock mutex
	{
		std::lock_guard<std::mutex> lockGuard{ m_Mutex };

		// Store info
		AudioInfo info{};
		info.soundID = ID;
		info.threadInstruction = ResumeSFX;

		m_AudioQueue.push(info);
	}

	// Notify
	m_ConditionVariable.notify_all();
}
void dae::SDLSoundSystem::SetVolumeAudio(unsigned int ID, int volume)
{
	// Lock mutex
	{
		std::lock_guard<std::mutex> lockGuard{ m_Mutex };

		// Store info
		AudioInfo info{};
		info.soundID = ID;
		info.volume = volume;
		info.threadInstruction = SetVolumeSFX;

		m_AudioQueue.push(info);
	}

	// Notify
	m_ConditionVariable.notify_all();
}
#pragma endregion

void dae::SDLSoundSystem::SetID(unsigned int ID, const std::string& resourceName)
{
	// Lock mutex
	{
		std::lock_guard<std::mutex> lockGuard{ m_Mutex };

		// Store info
		AudioInfo info{};
		info.soundID = ID;
		info.resourceName = resourceName;
		info.threadInstruction = LoadSFX;

		m_AudioQueue.push(info);
	}

	// Notify
	m_ConditionVariable.notify_all();
}

bool dae::SDLSoundSystem::IsValid(unsigned int ID, bool printError)
{
	if (m_AudioFiles.find(ID) == m_AudioFiles.end())
	{
		if (printError) std::cout << "Error: tried to load non-existent soundID" << std::endl;
		return false;
	}
	else return true;
}

void dae::SDLSoundSystem::Load(unsigned int ID, const std::string& resourceName)
{
	m_AudioFiles[ID] = ResourceManager::GetInstance().LoadSound(resourceName);
}
void dae::SDLSoundSystem::Play(unsigned int ID, int volume, int loops)
{
	// Check validity
	if (IsValid(ID) == false) return;

	// Set volume and play
	m_AudioFiles[ID]->SetVolume(volume);
	m_AudioFiles[ID]->Play(loops);
}
void dae::SDLSoundSystem::Pause(unsigned int ID)
{
	if (IsValid(ID)) m_AudioFiles[ID]->Pause();
}
void dae::SDLSoundSystem::Resume(unsigned int ID)
{
	if (IsValid(ID)) m_AudioFiles[ID]->Resume();
}
void dae::SDLSoundSystem::SetVolume(unsigned int ID, int volume)
{
	if (IsValid(ID)) m_AudioFiles[ID]->SetVolume(volume);
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
		m_IsHandlingAudio = true;

		// If is not being destroyed
		if (m_IsBeingDestroyed == false)
		{
			// While queue is not empty
			while (m_AudioQueue.empty() == false)
			{
				// Get first file
				const AudioInfo currentAudioFile{ m_AudioQueue.front() };

				// Check instruction
				switch (currentAudioFile.threadInstruction)
				{
				case dae::SDLSoundSystem::LoadSFX:
					Load(currentAudioFile.soundID, currentAudioFile.resourceName);
					break;
			
				case dae::SDLSoundSystem::PlaySFX:
					Play(currentAudioFile.soundID, currentAudioFile.volume, currentAudioFile.loops);
					break;

				case dae::SDLSoundSystem::PauseSFX:
					Pause(currentAudioFile.soundID);
					break;

				case dae::SDLSoundSystem::ResumeSFX:
					Resume(currentAudioFile.soundID);
					break;
				}

				// Pop file
				m_AudioQueue.pop();
			}
		}

		m_IsHandlingAudio = false;
	}
}