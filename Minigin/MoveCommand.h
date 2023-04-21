#pragma once
#include "GameObjectCommand.h"
#include <glm/vec2.hpp>

namespace dae
{
	class MoveCommand final : public GameObjectCommand
	{
	public:
		// Rule of five
		explicit MoveCommand(GameObject* pActor, glm::vec2 movementDirection, float movementSpeed);
		virtual ~MoveCommand() = default;

		MoveCommand(const MoveCommand& other) = delete;
		MoveCommand(MoveCommand&& other) = delete;
		MoveCommand& operator=(const MoveCommand& other) = delete;
		MoveCommand& operator=(MoveCommand&& other) = delete;

		// Public functions
		virtual void Execute(float deltaTime) override;

		void SetMovementSpeed(float movementSpeed) { m_MovementSpeed = movementSpeed; }
		float GetMovementSpeed() const { return m_MovementSpeed; }

	private:
		// Member variables
		// ----------------
		const glm::vec2 m_MovementDirection{};
		float m_MovementSpeed{};
	};
}