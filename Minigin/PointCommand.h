#pragma once
#include "GameObjectCommand.h"
#include "EventManager.h"

namespace dae
{
	class PointComponent;

	class PointCommand final : public GameObjectCommand
	{
	public:
		// Rule of five
		explicit PointCommand(GameObject* pActor);
		virtual ~PointCommand() = default;

		PointCommand(const PointCommand& other) = delete;
		PointCommand(PointCommand&& other) = delete;
		PointCommand& operator=(const PointCommand& other) = delete;
		PointCommand& operator=(PointCommand&& other) = delete;

		// Public functions
		virtual void Execute(float deltaTime) override;

	private:
		// Member variables
		PointComponent* m_pPointComponent{ nullptr };
	};
}