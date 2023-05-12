#pragma once
#include "Command.h"

class PauseCommand final : public dae::Command
{
public:
	// Rule of five
	PauseCommand();
	virtual ~PauseCommand() override = default;

	PauseCommand(const PauseCommand& other) = delete;
	PauseCommand(PauseCommand&& other) = delete;
	PauseCommand& operator=(const PauseCommand& other) = delete;
	PauseCommand& operator=(PauseCommand&& other) = delete;

	// Functionality
	virtual void Execute(float deltaTime) override;

private:
	// Member variables
	unsigned int m_PauseID{};
};

