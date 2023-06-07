#pragma once
#include "RenderTextureComponent.h"

namespace dae
{
	class AnimationComponent final : public RenderTextureComponent
	{
	public:
		// Rule of five
		explicit AnimationComponent(dae::GameObject* pParentObject);
		virtual ~AnimationComponent() override = default;

		AnimationComponent(const AnimationComponent& other) = delete;
		AnimationComponent(AnimationComponent&& other) = delete;
		AnimationComponent& operator=(const AnimationComponent& other) = delete;
		AnimationComponent& operator=(AnimationComponent&& other) = delete;

		// Functionality
		virtual void Render() const override;

	private:
		// Member variables
		utils::Rect m_SrcRect{};
	};
}