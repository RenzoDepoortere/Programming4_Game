#pragma once
#include "SceneCommand.h"
#include "GameObject.h"

#include <memory>

namespace dae
{
	class RemoveObjectCommand final : public SceneCommand
	{
	public:
		// Rule of five
		explicit RemoveObjectCommand(Scene* pScene, GameObject* pToDelete);
		virtual ~RemoveObjectCommand() = default;

		RemoveObjectCommand(const RemoveObjectCommand& other) = delete;
		RemoveObjectCommand(RemoveObjectCommand&& other) = delete;
		RemoveObjectCommand& operator=(const RemoveObjectCommand& other) = delete;
		RemoveObjectCommand& operator=(RemoveObjectCommand&& other) = delete;

		// Public functions
		virtual void Execute(float deltaTime) override;
	
	private:
		// Member variables
		// ----------------
		GameObject* m_pGameObject{ nullptr };
	};
}