#pragma once

#include <map>
#include <memory>
#include <SDL.h>
#include <array>
#include <vector>

#include "Singleton.h"
#include "Controller.h"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:

		// Enum classes
		enum class ControllerButton
		{
			// Main
			Start = 0x0010,
			Select = 0x0020,
			//Thumbs
			LeftThumb = 0x0040,
			RightThumb = 0x0080,
			// Buttons
			ButtonA = 0x1000,
			ButtonB = 0x2000,
			ButtonX = 0x4000,
			ButtonY = 0x8000,
			// Back Buttons
			LeftTrigger = 0x0100,
			RightTrigger = 0x0200,
			// D-Pad
			DPadUp = 0x0001,
			DPadDown = 0x0002,
			DPadLeft = 0x0004,
			DPadRight = 0x0008,
			// Default
			None = 0
		};

		// Public functions
		bool ProcessInput(float deltaTime);

		bool IsPressed(unsigned long controllerIndex, ControllerButton controllerButton) const;
		bool IsReleased(unsigned long controllerIndex, ControllerButton controllerButton) const;
		bool IsDown(unsigned long controllerIndex, ControllerButton controllerButton) const;

		glm::vec2 GetLeftThumbInput(unsigned long controllerIndex) const;
		glm::vec2 GetRightThumbInput(unsigned long controllerIndex) const;

		float GetLeftTriggerInput(unsigned long controllerIndex) const;
		float GetRightTriggerInput(unsigned long controllerIndex) const;

		bool IsPressed(SDL_Scancode keyboardButton) const;
		bool IsReleased(SDL_Scancode keyboardButton) const;
		bool IsDown(SDL_Scancode keyboardButton) const;

		unsigned long AddController();
		void RemoveController(unsigned long controllerIndex);

	private:
		// Member variables
		// ----------------

		// Controllers
		unsigned long m_ControllerIndex{};
		std::map<unsigned long, std::unique_ptr<Controller>> m_Controllers;

		// Keyboard
		std::vector<SDL_Scancode> m_Pressed{};
		std::vector<SDL_Scancode> m_Released{};

		// Member functions
		// ----------------
		void ProcessControllers();
		bool ProcessKeyboard();
	};

}
