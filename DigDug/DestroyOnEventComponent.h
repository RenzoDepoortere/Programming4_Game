#pragma once
#include "Component.h"
#include "Observer.h"

class DestroyOnEventComponent final : public Component, public dae::Observer<float>
{
public:
	// Rule of Five
	explicit DestroyOnEventComponent(dae::GameObject* pParentObject);
	virtual ~DestroyOnEventComponent() override;

	DestroyOnEventComponent(DestroyOnEventComponent&& other) = delete;
	DestroyOnEventComponent& operator=(const DestroyOnEventComponent& other) = delete;
	DestroyOnEventComponent& operator=(DestroyOnEventComponent&& other) = delete;

	// Functionality
	void SetEvent(unsigned int eventID);

	// Observer
	virtual void HandleEvent(unsigned int eventID, float deltaTime);
	virtual void OnSubjectDestroy();

private:
	unsigned int m_EventID{};
};