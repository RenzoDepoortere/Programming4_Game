#pragma once
#include "Command.h"

class MuteCommand final : public dae::Command
{
public:
	// Rule of five
	MuteCommand() = default;
	virtual ~MuteCommand() override = default;

	MuteCommand(const MuteCommand& other) = delete;
	MuteCommand(MuteCommand&& other) = delete;
	MuteCommand& operator=(const MuteCommand& other) = delete;
	MuteCommand& operator=(MuteCommand&& other) = delete;

	// Functionality
	virtual void Execute(float deltaTime) override;

private:
	bool m_IsMuted{ false };
};

