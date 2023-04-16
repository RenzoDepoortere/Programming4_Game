#pragma once
#include "GameObjectCommand.h"

namespace dae
{
	class HealthComponent;

	class KillCommand final : public GameObjectCommand
	{
	public:
		// Rule of five
		explicit KillCommand(GameObject* pActor);
		virtual ~KillCommand() = default;

		KillCommand(const KillCommand& other) = delete;
		KillCommand(KillCommand&& other) = delete;
		KillCommand& operator=(const KillCommand& other) = delete;
		KillCommand& operator=(KillCommand&& other) = delete;

		// Public functions
		virtual void Execute(float deltaTime) override;

	private:
		// Member variables
		HealthComponent* m_pHealthComponent{ nullptr };
	};
}