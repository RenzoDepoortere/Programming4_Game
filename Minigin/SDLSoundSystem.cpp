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
void dae::SDLSoundSystem::PlayAudio(unsigned int ID, int volume, int loops, int channel)
{
	// Lock mutex
	{
		std::lock_guard<std::mutex> lockGuard{ m_Mutex };

		// Store info
		AudioInfo info{};
		info.soundID = ID;
		info.volume = volume;
		info.loops = loops;
		info.channel = channel;
		info.threadInstruction = PlaySFX;

		m_AudioQueue.push(info);
	}

	// Notify
	m_ConditionVariable.notify_all();
}
bool dae::SDLSoundSystem::IsPlayingAudio(unsigned int ID, int channel)
{
	// Wait untill the other thread is done processing the audioFiles
	while (m_IsHandlingAudio)
	{
	}

	if (IsValid(ID)) return m_AudioFiles[ID]->IsPlaying(channel);
	else			 return false;
}
void dae::SDLSoundSystem::PauseAudio(unsigned int ID, int channel)
{
	// Lock mutex
	{
		std::lock_guard<std::mutex> lockGuard{ m_Mutex };

		// Store info
		AudioInfo info{};
		info.soundID = ID;
		info.threadInstruction = PauseSFX;
		info.channel = channel;

		m_AudioQueue.push(info);
	}

	// Notify
	m_ConditionVariable.notify_all();
}
bool dae::SDLSoundSystem::IsPausedAudio(unsigned int ID, int channel)
{
	// Wait untill the other thread is done processing the audioFiles
	while (m_IsHandlingAudio)
	{
	}

	if (IsValid(ID)) return m_AudioFiles[ID]->IsPaused(channel);
	else			 return false;
}
void dae::SDLSoundSystem::ResumeAudio(unsigned int ID, int channel)
{
	// Lock mutex
	{
		std::lock_guard<std::mutex> lockGuard{ m_Mutex };

		// Store info
		AudioInfo info{};
		info.soundID = ID;
		info.threadInstruction = ResumeSFX;
		info.channel = channel;

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
void dae::SDLSoundSystem::Play(unsigned int ID, int volume, int loops, int channel)
{
	// Check validity
	if (IsValid(ID) == false) return;

	// Set volume and play
	m_AudioFiles[ID]->SetVolume(volume);
	m_AudioFiles[ID]->Play(loops, channel);
}
void dae::SDLSoundSystem::Pause(unsigned int ID, int channel)
{
	if (IsValid(ID)) m_AudioFiles[ID]->Pause(channel);
}
void dae::SDLSoundSystem::Resume(unsigned int ID, int channel)
{
	if (IsValid(ID)) m_AudioFiles[ID]->Resume(channel);
}
void dae::SDLSoundSystem::SetVolume(unsigned int ID, int volume)
{
	if (IsValid(ID)) m_AudioFiles[ID]->SetVolume(volume);
}

void dae::SDLSoundSystem::AudioThread()
{
	// Keep looping, till end of program
	while (m_IsBeingDestroyed == false)
	{
		// Unique lock mutex
		std::unique_lock<std::mutex> uniqueLock{ m_Mutex };

		// Wait for notification
		m_ConditionVariable.wait(uniqueLock, [&]() {return m_AudioQueue.size() > 0 || m_IsBeingDestroyed; });
		m_IsHandlingAudio = true;

		// If is not being destroyed
		if (m_IsBeingDestroyed == false)
		{
			// Get first file and pop
			const AudioInfo currentAudioFile{ m_AudioQueue.front() };
			m_AudioQueue.pop();

			// Unlock
			uniqueLock.unlock();

			// Check instruction
			switch (currentAudioFile.threadInstruction)
			{
			case dae::SDLSoundSystem::LoadSFX:
				Load(currentAudioFile.soundID, currentAudioFile.resourceName);
				break;
			
			case dae::SDLSoundSystem::PlaySFX:
				Play(currentAudioFile.soundID, currentAudioFile.volume, currentAudioFile.loops, currentAudioFile.channel);
				break;

			case dae::SDLSoundSystem::PauseSFX:
				Pause(currentAudioFile.soundID, currentAudioFile.channel);
				break;

			case dae::SDLSoundSystem::ResumeSFX:
				Resume(currentAudioFile.soundID, currentAudioFile.channel);
				break;
			}
		}

		m_IsHandlingAudio = false;
	}
}