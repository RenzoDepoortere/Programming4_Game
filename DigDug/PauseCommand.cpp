#include "PauseCommand.h"
#include "ServiceLocator.h"

#include <iostream>

PauseCommand::PauseCommand()
{
	// Get SFX ID
	const std::string fileName{ "Sound/Pause_SFX.wav" };
	m_PauseID = dae::ServiceLocator::GetSoundSystem().SetID(fileName);
}

void PauseCommand::Execute(float)
{
	// Play sound
	const int volume{ 100 };
	const int loops{ 0 };
	dae::ServiceLocator::GetSoundSystem().Play(m_PauseID, volume, loops);
}