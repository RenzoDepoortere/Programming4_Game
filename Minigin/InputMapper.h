#pragma once
#include "InputManager.h"
#include <map>
#include "Command.h"

namespace dae
{
	class GameObject;

	class InputMapper final : public Singleton<InputMapper>
	{
	public:

		// Enum classes
		// ------------
		enum class KeyState
		{
			Press, Release, Hold
		};

		// Public functions
		// ----------------
		using controllerInput = std::pair<unsigned long, InputManager::ControllerButton>;
		using inputKey = std::pair<SDL_Scancode, controllerInput>;
		using keyBinding = std::pair<KeyState, std::unique_ptr<Command>>;

		void IterateMappedInput(float deltaTime);
		void MapInputKey(const inputKey& inputKey, KeyState keyState, std::unique_ptr<Command>&& pCommand);

		// Todo: create a function that allows to delete mapped inputKeys

	private:
		// Member variables
		// ----------------
		std::map<inputKey, keyBinding> m_KeyBindings{};

	};
}