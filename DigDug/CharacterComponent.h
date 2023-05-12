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

private:
	// Member variables
	grid::GridComponent* m_pGrid{ nullptr };

	// Member functions
	void RemoveDirt();
};

