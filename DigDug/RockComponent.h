#pragma once
#include "Component.h"
#include "MoveCommand.h"

namespace dae
{
	class GameObject;
	class AnimationComponent;
}

namespace grid
{
	class GridComponent;
	struct Cell;
}

class RockComponent final : public Component
{
public:
	// Rule of five
	explicit RockComponent(dae::GameObject* pParentObject);
	virtual ~RockComponent() override = default;

	RockComponent(const RockComponent& other) = delete;
	RockComponent(RockComponent&& other) = delete;
	RockComponent& operator=(const RockComponent& other) = delete;
	RockComponent& operator=(RockComponent&& other) = delete;

	// Functionality
	virtual void Update(float deltaTime) override;

	void SetGrid(grid::GridComponent* pGrid) { m_pGrid = pGrid; }
	void SetAnimationComponent(dae::AnimationComponent* pAnimationComponent) { m_pAnimationComponent = pAnimationComponent; }

private:
	// Enums
	// -----
	enum RockState
	{
		Passive, Unstable, Falling, Destroyed
	};

	// Member variables
	// ----------------
	grid::GridComponent* m_pGrid{ nullptr };
	dae::AnimationComponent* m_pAnimationComponent{ nullptr };
	RockState m_CurrentRockState{ Passive };

	int m_RotationDirection{ 1 };
	float m_CurrentRumbleTime{};

	std::unique_ptr<dae::MoveCommand> pMoveCommand{ nullptr };

	// Member functions
	// ----------------
	void BottomCellGone();
	void Rumble(float deltaTime);
	void Fall(float deltaTime);
	void Destroy(float deltaTime);

	grid::Cell* GetCellBelow() const;
};

