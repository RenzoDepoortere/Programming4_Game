#include "AudioFile.h"
#include "SDL_mixer.h"

dae::AudioFile::AudioFile(void* pChunk)
	: m_pChunk{ pChunk }
{
}

dae::AudioFile::~AudioFile()
{
	Mix_FreeChunk(static_cast<Mix_Chunk*>(m_pChunk));
}

// Note: give each chunk their own channel if necessary (max 8 channels in mixer)

void dae::AudioFile::Play(int loops)
{
	Mix_PlayChannel(0, static_cast<Mix_Chunk*>(m_pChunk), loops);
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
	Mix_VolumeChunk(static_cast<Mix_Chunk*>(m_pChunk), volume);
}