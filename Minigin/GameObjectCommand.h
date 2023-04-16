#pragma once
#include "Command.h"

namespace dae
{
	class GameObject;

	class GameObjectCommand : public Command
	{
	public:
		// Rule of five
		explicit GameObjectCommand(GameObject* pActor);
		virtual ~GameObjectCommand() = default;

		GameObjectCommand(const GameObjectCommand& other) = delete;
		GameObjectCommand(GameObjectCommand&& other) = delete;
		GameObjectCommand& operator=(const GameObjectCommand& other) = delete;
		GameObjectCommand& operator=(GameObjectCommand&& other) = delete;

		// Public functions
		virtual void Execute(float deltaTime) = 0;

		protected:
			// Protected functions
			GameObject* GetGameObject() const { return m_pActor; }

		private:
			// Member variables
			GameObject* m_pActor{ nullptr };
	};

}