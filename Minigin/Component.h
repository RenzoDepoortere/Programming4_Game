#pragma once
#include <memory>
#include "Utils.h"

namespace dae
{
	class GameObject;
}

class Component
{
public:
	explicit Component(dae::GameObject* pParentObject);
	virtual ~Component() = default;

	Component(const Component& other) = delete;
	Component(Component&& other) = delete;
	Component& operator=(const Component& other) = delete;
	Component& operator=(Component&& other) = delete;

	virtual void Update(float deltaTime);
	virtual void FixedUpdate(float deltaTime);
	virtual void Render() const;
	virtual void RenderImGUI();

protected:
	dae::GameObject* GetGameObject() const;

private:
	// Member variables
	dae::GameObject* m_pParentObject{};
};
