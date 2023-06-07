#pragma once
#include "Component.h"

namespace dae
{
	class GameObject;
	class RenderTextureComponent;
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

	void StartThrow();
	void SetThrowSpeed(float throwSpeed) { m_ThrowSpeed = throwSpeed; }

	void SetRenderTextureComponent(dae::RenderTextureComponent* pRenderTextureComponent);

	bool GetIsThrowing() const { return m_IsThrowing; }

private:
	// Member variables
	// ----------------
	bool m_IsThrowing{ false };
	
	utils::Rect m_SrcRect{};
	int m_TextureWidth{};
	float m_TextureFill{};

	dae::RenderTextureComponent* m_pRenderTextureComponent{ nullptr };

	float m_ThrowSpeed{};

	// Member functions
	// ----------------
	void HandleRopeExtension(float deltaTime);
};