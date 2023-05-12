#include "AudioFile.h"
#include "SDL_mixer.h"

dae::AudioFile::AudioFile(Mix_Chunk* pChunk)
	: m_pChunk{ pChunk }
{
}

dae::AudioFile::~AudioFile()
{
	Mix_FreeChunk(m_pChunk);
}

// Note: give each chunk their own channel if necessary (max 8 channels in mixer)

void dae::AudioFile::Play(int loops)
{
	Mix_PlayChannel(0, m_pChunk, loops);
}
bool dae::AudioFile::IsPlaying()
{
	if (Mix_Playing(0) != 0) return true;
	else					 return false;
}
void dae::AudioFile::Pause()
{
	Mix_Pause(0);
}
bool dae::AudioFile::IsPaused()
{
	if (Mix_Paused(0) == 1) return true;
	else					return false;
}
void dae::AudioFile::Resume()
{
	Mix_Resume(0);
}
void dae::AudioFile::SetVolume(int volume)
{
	Mix_VolumeChunk(m_pChunk, volume);
}