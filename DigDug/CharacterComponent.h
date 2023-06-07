#pragma once
#include "Component.h"
#include "MoveCommand.h"
#include "CharacterState.h"

#include <memory>
#include <array>

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
	grid::GridComponent* GetGrid() const { return m_pGrid; }

	void SetAnimationComponent(dae::AnimationComponent* pAnimationComponent) { m_pAnimationComponent = pAnimationComponent; }
	dae::AnimationComponent* GetAnimationComponent() const { return m_pAnimationComponent; }

	void SetControllerID(unsigned long controllerID) { m_ControllerID = controllerID; }

private:
	// Member variables
	// ----------------
	grid::GridComponent* m_pGrid{ nullptr };
	dae::AnimationComponent* m_pAnimationComponent{ nullptr };

	unsigned long m_ControllerID{};

	Player::CharacterState* m_pCurrentState{};
	std::array<std::unique_ptr<Player::CharacterState>, Player::NR_STATES> m_pPlayerStates{};

	bool m_InitializedStates{ false };

	// Member functions
	// ----------------
	void InitStates();
};

