#include "PauseCommand.h"
#include "ServiceLocator.h"
#include "EventManager.h"
#include "EventsEnum.h"

#include <iostream>

PauseCommand::PauseCommand()
{
	// Get SFX ID
	const std::string fileName{ "Sound/Pause_SFX.wav" };
	m_PauseID = dae::ServiceLocator::GetSoundSystem().SetID(fileName);
}

void PauseCommand::Execute(float)
{
	const int volume{ 100 };
	const int loops{ 0 };

	// Sent playEvent
	dae::EventManager<unsigned int, int, int>::GetInstance().SendEvent(event::PauseMenu, m_PauseID, volume, loops);
}