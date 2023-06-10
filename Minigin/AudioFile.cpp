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

void dae::AudioFile::Play(int loops, int channel)
{
	Mix_PlayChannel(channel, static_cast<Mix_Chunk*>(m_pChunk), loops);
}
bool dae::AudioFile::IsPlaying(int channel)
{
	if (Mix_Playing(channel) != 0) return true;
	else					 return false;
}
void dae::AudioFile::Pause(int channel)
{
	Mix_Pause(channel);
}
bool dae::AudioFile::IsPaused(int channel)
{
	if (Mix_Paused(channel) == 1) return true;
	else					return false;
}
void dae::AudioFile::Resume(int channel)
{
	Mix_Resume(channel);
}
void dae::AudioFile::SetVolume(int volume)
{
	Mix_VolumeChunk(static_cast<Mix_Chunk*>(m_pChunk), volume);
}