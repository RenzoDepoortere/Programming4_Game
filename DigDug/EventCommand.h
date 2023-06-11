#pragma once
#include "Command.h"

class EventCommand final : public dae::Command
{
public:
	// Rule of five
	EventCommand(unsigned int eventID);
	virtual ~EventCommand() override = default;

	EventCommand(const EventCommand& other) = delete;
	EventCommand(EventCommand&& other) = delete;
	EventCommand& operator=(const EventCommand& other) = delete;
	EventCommand& operator=(EventCommand&& other) = delete;

	// Functionality
	virtual void Execute(float deltaTime) override;

private:
	unsigned int m_EventID{};
};

