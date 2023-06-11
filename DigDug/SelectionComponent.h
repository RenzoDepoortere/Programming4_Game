#pragma once
#include "Component.h"
#include "Observer.h"

#include <vector>

class SelectionComponent final : public Component, public dae::Observer<float>
{
public:
	// Rule of five
	explicit SelectionComponent(dae::GameObject* pParentObject);
	virtual ~SelectionComponent() override;

	SelectionComponent(const SelectionComponent& other) = delete;
	SelectionComponent(SelectionComponent&& other) = delete;
	SelectionComponent& operator=(const SelectionComponent& other) = delete;
	SelectionComponent& operator=(SelectionComponent&& other) = delete;

	// Functionality
	void SetPositions(const std::vector<float>& positions) { m_Positions = positions; }

	// Observer
	virtual void HandleEvent(unsigned int eventID, float deltaTime) override;
	virtual void OnSubjectDestroy() override;

private:
	// Member variables
	// ----------------
	int m_CurrentPos{};
	std::vector<float> m_Positions{};

	float m_XPos{};
};