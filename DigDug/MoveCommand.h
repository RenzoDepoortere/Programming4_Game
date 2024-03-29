#pragma once
#include "GameObjectCommand.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace grid
{
	class GridComponent;
}

namespace dae
{
	class MoveCommand final : public GameObjectCommand
	{
	public:
		// Rule of five
		explicit MoveCommand(GameObject* pActor, glm::vec2 movementDirection, float movementSpeed, grid::GridComponent* pGrid = nullptr, bool checkDirt = true);
		virtual ~MoveCommand() = default;

		MoveCommand(const MoveCommand& other) = delete;
		MoveCommand(MoveCommand&& other) = delete;
		MoveCommand& operator=(const MoveCommand& other) = delete;
		MoveCommand& operator=(MoveCommand&& other) = delete;

		// Functionality
		virtual void Execute(float deltaTime) override;

		void SetMovementSpeed(float movementSpeed) { m_MovementSpeed = movementSpeed; }
		float GetMovementSpeed() const { return m_MovementSpeed; }

		void SetMovementDirection(const glm::vec2& movementDirection) { m_MovementDirection = movementDirection; }
		glm::vec2 GetMovementDirection() const { return m_MovementDirection; }

		void SetMoveInGrid(bool moveInGrid) { m_MoveInGrid = moveInGrid; }
		bool GetMoveInGrid() const { return m_MoveInGrid; }

	private:
		// Member variables
		// ----------------
		glm::vec2 m_MovementDirection{};
		float m_MovementSpeed{};

		grid::GridComponent* m_pGrid{ nullptr };
		bool m_CheckDirt{};
		bool m_MoveInGrid{};

		// Member functions
		// ----------------
		void GridMovement(glm::vec2& desiredDirection, const glm::vec3& startActorPos);
	};
}