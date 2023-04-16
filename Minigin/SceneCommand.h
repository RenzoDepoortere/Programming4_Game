#pragma once
#include "Command.h"

namespace dae
{
	class Scene;

	class SceneCommand : public Command
	{
	public:
		// Rule of five
		explicit SceneCommand(Scene* pScene);
		virtual ~SceneCommand() = default;

		SceneCommand(const SceneCommand& other) = delete;
		SceneCommand(SceneCommand&& other) = delete;
		SceneCommand& operator=(const SceneCommand& other) = delete;
		SceneCommand& operator=(SceneCommand&& other) = delete;

		// Public functions
		virtual void Execute(float deltaTime) = 0;

	protected:
		// Protected functions
		Scene* GetScene() const { return m_pScene; }

	private:
		// Member variables
		Scene* m_pScene{ nullptr };
	};
}