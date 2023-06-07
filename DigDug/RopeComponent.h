#pragma once
#include "Component.h"

namespace dae
{
	class GameObject;
}

class RopeComponent final : public Component
{
public:
	// Rule of Five
	explicit RopeComponent(dae::GameObject* pParentObject);
	virtual ~RopeComponent() override = default;

	RopeComponent(RopeComponent&& other) = delete;
	RopeComponent& operator=(const RopeComponent& other) = delete;
	RopeComponent& operator=(RopeComponent&& other) = delete;

	// Functionality
	virtual void Update(float deltaTime) override;
	

private:
	
};