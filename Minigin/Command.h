#pragma once

namespace dae
{
	class Command
	{
	public:
		// Rule of five
		explicit Command() = default;
		virtual ~Command() = default;

		Command(const Command& other) = delete;
		Command(Command&& other) = delete;
		Command& operator=(const Command& other) = delete;
		Command& operator=(Command&& other) = delete;

		// Public functions
		virtual void Execute(float deltaTime) = 0;
	};

}