#include "EventCommand.h"

#include "EventManager.h"

EventCommand::EventCommand(unsigned int eventID)
	: m_EventID{ eventID }
{
}

void EventCommand::Execute(float /*deltaTime*/)
{
	dae::EventManager<>::GetInstance().SendEvent(m_EventID);
}