#include "InputMapper.h"

void dae::InputMapper::IterateMappedInput(float deltaTime)
{
	// Check for each mapped key if condition was met
	for (const auto& currentMappedKey : m_KeyBindings)
	{
		// variables
		Command* pCommand{ currentMappedKey.second.second.get() };
		const KeyState mappedKeyState{ currentMappedKey.second.first };
		const SDL_Scancode keyboardKey{ currentMappedKey.first.first };
		const controllerInput controllerButton{ currentMappedKey.first.second };

		bool conditionMet{ false };

		// Check corresponding keyState
		switch (mappedKeyState)
		{
		case KeyState::Press:
			conditionMet = dae::InputManager::GetInstance().IsPressed(controllerButton.first, controllerButton.second);
			if (conditionMet == false) conditionMet = dae::InputManager::GetInstance().IsPressed(keyboardKey);
			break;

		case KeyState::Release:
			conditionMet = dae::InputManager::GetInstance().IsReleased(controllerButton.first, controllerButton.second);
			if (conditionMet == false) conditionMet = dae::InputManager::GetInstance().IsReleased(keyboardKey);
			break;

		case KeyState::Hold:
			conditionMet = dae::InputManager::GetInstance().IsDown(controllerButton.first, controllerButton.second);
			if (conditionMet == false) conditionMet = dae::InputManager::GetInstance().IsDown(keyboardKey);
			break;
		}

		// If condition met, execute command
		if (conditionMet) pCommand->Execute(deltaTime);
	}
}

void dae::InputMapper::MapInputKey(const inputKey& inputKey, KeyState keyState, std::unique_ptr<Command>&& pCommand)
{
	// Map keyState and command to inputKeys
	m_KeyBindings[inputKey] = std::make_pair(keyState, std::move(pCommand));
}