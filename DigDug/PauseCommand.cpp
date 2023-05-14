#include "PauseCommand.h"
#include "ServiceLocator.h"
#include "EventManager.h"
#include "EventsEnum.h"

#include <iostream>

PauseCommand::PauseCommand()
{
	// Get SFX ID
	const std::string fileName{ "Sound/Pause_SFX.wav" };
	dae::ServiceLocator::GetSoundSystem().SetID(event::PauseMenu, fileName);
}

void PauseCommand::Execute(float)
{
	const int volume{ 100 };
	const int loops{ 0 };

	// Sent playEvent
	dae::EventManager<int, int>::GetInstance().SendEvent(event::PauseMenu, volume, loops);
}