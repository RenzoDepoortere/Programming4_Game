#pragma once
#include "Component.h"

#include <vector>

class SelectionComponent final : public Component
{
public:
	// Rule of five
	explicit SelectionComponent(dae::GameObject* pParentObject);
	virtual ~SelectionComponent() override = default;

	SelectionComponent(const SelectionComponent& other) = delete;
	SelectionComponent(SelectionComponent&& other) = delete;
	SelectionComponent& operator=(const SelectionComponent& other) = delete;
	SelectionComponent& operator=(SelectionComponent&& other) = delete;

	// Functionality
	virtual void Update(float deltaTime) override;

	void SetPositions(const std::vector<float>& positions) { m_Positions = positions; }

private:
	// Member variables
	// ----------------
	int m_CurrentPos{};
	std::vector<float> m_Positions{};

	float m_XPos{};
};