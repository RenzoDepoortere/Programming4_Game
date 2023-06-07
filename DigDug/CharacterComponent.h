#pragma once
#include "Component.h"
#include "MoveCommand.h"

#include <memory>

namespace dae
{
	class GameObject;
	class AnimationComponent;
}

namespace grid
{
	class GridComponent;
}

class CharacterComponent final : public Component
{
public:
	// Rule of five
	explicit CharacterComponent(dae::GameObject* pParentObject);
	virtual ~CharacterComponent() override = default;

	CharacterComponent(const CharacterComponent& other) = delete;
	CharacterComponent(CharacterComponent&& other) = delete;
	CharacterComponent& operator=(const CharacterComponent& other) = delete;
	CharacterComponent& operator=(CharacterComponent&& other) = delete;

	// Functionality
	virtual void Update(float deltaTime) override;

	void SetGrid(grid::GridComponent* pGrid) { m_pGrid = pGrid; }
	void SetAnimationComponent(dae::AnimationComponent* pAnimationComponent) { m_pAnimationComponent = pAnimationComponent; }

	void SetControllerID(unsigned long controllerID) { m_ControllerID = controllerID; }

private:
	// Member variables
	grid::GridComponent* m_pGrid{ nullptr };
	dae::AnimationComponent* m_pAnimationComponent{ nullptr };

	unsigned long m_ControllerID{};

	bool m_MoveCommandInitialized{};
	std::unique_ptr<dae::MoveCommand> m_pMoveCommand{};

	// Member functions
	void HandleInput(float deltaTime, unsigned long controlledID);	// This should be handled by states later
	void RemoveDirt();

	void InitMoveCommand();
};

