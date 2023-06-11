#pragma once
#include "Component.h"
#include "Observer.h"

#include <vector>
#include <functional>

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
	virtual void Update(float deltaTime) override;
	void SubscribeToEvents() { m_HasToSubscribe = true; }

	void SetPositions(const std::vector<float>& positions) { m_Positions = positions; }
	void SetXPos(float xPos) { m_XPos = xPos; }

	void SetActivateFunctions(const std::vector<std::function<void()>>& functions) { m_Functions = functions; }

	// Observer
	virtual void HandleEvent(unsigned int eventID, float deltaTime) override;
	virtual void OnSubjectDestroy() override;

private:
	// Member variables
	// ----------------
	int m_CurrentPos{};
	std::vector<float> m_Positions{};
	float m_XPos{};

	std::vector<std::function<void()>> m_Functions{};

	float m_Cooldown{};
	float m_MaxCooldown{};

	bool m_HasToSubscribe{};

	// Member functions
	// ----------------
	void Activate();
	void Subscribe();

	void GoDown();
	void GoUp();
};