#pragma once
#include "Component.h"

namespace dae
{
	class RenderTextureComponent;
}

class EnemyComponent;
class FireComponent final : public Component
{
public:
	// Rule of Five
	explicit FireComponent(dae::GameObject* pParentObject);
	virtual ~FireComponent() override = default;

	FireComponent(FireComponent&& other) = delete;
	FireComponent& operator=(const FireComponent& other) = delete;
	FireComponent& operator=(FireComponent&& other) = delete;

	// Functionality
	virtual void Update(float deltaTime) override;

	void SetActive(bool isActive, EnemyComponent* pEnemy);
	void SetRenderTextureComponent(dae::RenderTextureComponent* pRenderComponent) { m_pRenderComponent = pRenderComponent; }

private:
	// Member variables
	// ----------------
	dae::RenderTextureComponent* m_pRenderComponent{ nullptr };

	// Member functions
	// ----------------
	void CheckCollision();
};