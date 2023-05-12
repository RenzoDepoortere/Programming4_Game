#pragma once
#include "Component.h"

namespace dae
{
	class GameObject;
}

namespace grid
{
	class GridComponent;
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

private:
	// Enums
	enum RockState
	{
		Passive, Unstable, Falling, Destroyed
	};

	// Member variables
	grid::GridComponent* m_pGrid{ nullptr };
	RockState m_CurrentRockState{ Passive };

	// Member functions
	void BottomCellGone();
	void Rumble(float deltaTime);
};

