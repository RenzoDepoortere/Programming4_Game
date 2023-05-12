#include "SDLSoundSystem.h"
#include "SDL_mixer.h"
#include "AudioFile.h"
#include "ResourceManager.h"

#include <iostream>

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

unsigned int dae::SDLSoundSystem::SetID(const std::string& resourceName)
{
	m_IDs[m_NextFreeID] = resourceName;
	return m_NextFreeID++;
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

void dae::SDLSoundSystem::HandleEvent(int /*eventID*/, unsigned int /*soundID*/, int /*volume*/, int /*loops = 1*/)
{

}