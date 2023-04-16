#include "InputManager.h"
#include "backends/imgui_impl_sdl2.h"
#include "Controller.h"
#include "InputMapper.h"

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <thread>
#include <iostream>
#include <algorithm>

// Basic input of keyboard --> always and each controller individually --> processes input for each controller here
bool dae::InputManager::ProcessInput(float deltaTime)
{
	// Controllers
	ProcessControllers();

	// Keyboard
	const bool continueProgram = ProcessKeyboard();

	// Iterate through mapped keys
	dae::InputMapper::GetInstance().IterateMappedInput(deltaTime);

	// Return
	return continueProgram;
}

bool dae::InputManager::IsPressed(unsigned long controllerIndex, ControllerButton controllerButton) const
{
	if (controllerButton == ControllerButton::None) return false;
	return m_Controllers.at(controllerIndex)->IsPressed(static_cast<unsigned int>(controllerButton));
}
bool dae::InputManager::IsReleased(unsigned long controllerIndex, ControllerButton controllerButton) const
{
	if (controllerButton == ControllerButton::None) return false;
	return m_Controllers.at(controllerIndex)->IsReleased(static_cast<unsigned int>(controllerButton));
}
bool dae::InputManager::IsDown(unsigned long controllerIndex, ControllerButton controllerButton) const
{
	if (controllerButton == ControllerButton::None) return false;
	return m_Controllers.at(controllerIndex)->IsDown(static_cast<unsigned int>(controllerButton));
}

glm::vec2 dae::InputManager::GetLeftThumbInput(unsigned long controllerIndex) const
{
	return m_Controllers.at(controllerIndex)->GetLeftThumbInput();
}
glm::vec2 dae::InputManager::GetRightThumbInput(unsigned long controllerIndex) const
{
	return m_Controllers.at(controllerIndex)->GetRightThumbInput();
}

float dae::InputManager::GetLeftTriggerInput(unsigned long controllerIndex) const
{
	return m_Controllers.at(controllerIndex)->GetLeftTriggerInput();
}
float dae::InputManager::GetRightTriggerInput(unsigned long controllerIndex) const
{
	return m_Controllers.at(controllerIndex)->GetRightTriggerInput();
}


bool dae::InputManager::IsPressed(SDL_Scancode keyboardButton) const
{
	// Find button in pressed container, return if not found
	return std::find(m_Pressed.cbegin(), m_Pressed.cend(), keyboardButton) != m_Pressed.cend();
}
bool dae::InputManager::IsReleased(SDL_Scancode keyboardButton) const
{
	// Find button in released container, return if not found
	return std::find(m_Released.cbegin(), m_Released.cend(), keyboardButton) != m_Released.cend();
}
bool dae::InputManager::IsDown(SDL_Scancode keyboardButton) const
{
	return SDL_GetKeyboardState(NULL)[keyboardButton];
}

unsigned long dae::InputManager::AddController()
{
	m_Controllers[m_ControllerIndex] = std::make_unique<Controller>(m_ControllerIndex);
	return m_ControllerIndex++;	// Return controllerIndex, +1 afterwards
}
void dae::InputManager::RemoveController(unsigned long controllerIndex)
{
	m_Controllers.erase(controllerIndex);
	--m_ControllerIndex;
}

void dae::InputManager::ProcessControllers()
{
	// Process input for each controller
	// ---------------------------------
	for (const auto& currentController : m_Controllers)
	{
		currentController.second->ProcessInput();
	}
}
bool dae::InputManager::ProcessKeyboard()
{
	// Empty containers
	// ----------------
	m_Pressed.clear();
	m_Released.clear();

	// Process input for keyboard and mouse
	// ------------------------------------
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		// Quit program
		switch (e.type)
		{
		case SDL_QUIT:
			return false;
			break;

		case SDL_KEYDOWN:
			if (e.key.keysym.sym == SDLK_ESCAPE) return false;

		// Store pressed/released keys	
			m_Pressed.push_back(e.key.keysym.scancode);
			break;

		case SDL_KEYUP:
			m_Released.push_back(e.key.keysym.scancode);
			break;
		}

		// Process event for ImGUI
		ImGui_ImplSDL2_ProcessEvent(&e);
	}


	// Return
	return true;
}