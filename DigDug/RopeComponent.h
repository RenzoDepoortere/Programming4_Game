#pragma once
#include "Component.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

namespace dae
{
	class GameObject;
	class RenderTextureComponent;
}

namespace grid
{
	class GridComponent;
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
	virtual void Render() const override;

	void StartThrow(unsigned int lookingDirection, const glm::vec3& startPos);
	void SetThrowSpeed(float throwSpeed) { m_ThrowSpeed = throwSpeed; }

	void SetRenderTextureComponent(dae::RenderTextureComponent* pRenderTextureComponent);
	void SetGrid(grid::GridComponent* pGrid) { m_pGrid = pGrid; }

	bool GetIsThrowing() const { return m_IsThrowing; }
	bool GetCaughtEnemy() const { return m_CaughtEnemy; }

private:
	// Member variables
	// ----------------
	bool m_IsThrowing{ false };
	bool m_CaughtEnemy{ false };

	utils::Rect m_SrcRect{};
	int m_TextureWidth{};
	float m_TextureFill{};

	dae::RenderTextureComponent* m_pRenderTextureComponent{ nullptr };
	grid::GridComponent* m_pGrid{ nullptr };

	float m_ThrowSpeed{};
	glm::vec2 m_MovementDirection{};
	glm::vec3 m_StartPosition{};

	// Member functions
	// ----------------
	void HandleRopeExtension(float deltaTime);
	void CheckCollision();
};