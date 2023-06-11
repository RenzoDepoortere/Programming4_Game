#pragma once
#include "Command.h"

class SkipLevelCommand final : public dae::Command
{
public:
	// Rule of five
	SkipLevelCommand() = default;
	virtual ~SkipLevelCommand() override = default;

	SkipLevelCommand(const SkipLevelCommand& other) = delete;
	SkipLevelCommand(SkipLevelCommand&& other) = delete;
	SkipLevelCommand& operator=(const SkipLevelCommand& other) = delete;
	SkipLevelCommand& operator=(SkipLevelCommand&& other) = delete;

	// Functionality
	virtual void Execute(float deltaTime) override;
};

