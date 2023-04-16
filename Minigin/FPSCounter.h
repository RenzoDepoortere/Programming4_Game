#pragma once
#include <string>
#include "Component.h"

namespace dae
{
	class GameObject;
	class TextComponent;

	class FPSCounter final : public Component
	{
	public:
		explicit FPSCounter(dae::GameObject* pParentObject);
		virtual ~FPSCounter() override = default;

		FPSCounter(const FPSCounter& other) = delete;
		FPSCounter(FPSCounter&& other) = delete;
		FPSCounter& operator=(const FPSCounter& other) = delete;
		FPSCounter& operator=(FPSCounter&& other) = delete;

		virtual void Update(float deltaTime) override;

	private:

		int m_FrameCounter{};
		float m_ElapsedTime{};
		float m_MaxCalculateTime{};

		TextComponent* m_pTextComponent{ nullptr };
	};
}

