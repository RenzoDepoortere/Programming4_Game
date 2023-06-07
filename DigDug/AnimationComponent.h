#pragma once
#include "RenderTextureComponent.h"

namespace dae
{
	// Remark: won't handle all possible combinations of spriteSheet-setup, only packed on 1 line
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
		virtual void Update(float deltaTime) override;
		virtual void Render() const override;

		void SetSingleSpriteSize(float spriteSize);
		void SetMaxFrames(int maxFrames) { m_MaxFrames = maxFrames; }
		void SetFramesPerSecond(int framesPerSecond) { m_FramesPerSecond = framesPerSecond; }

		bool PlayedOnce() const { return m_PlayedOnce; }

		bool GetPaused() const { return m_IsPaused; }
		void SetPaused(bool isPaused) { m_IsPaused = isPaused; }

	private:
		// Member variables
		utils::Rect m_SrcRect{};

		bool m_PlayedOnce{};
		bool m_IsPaused{};

		int m_FramesPerSecond{};
		float m_TimePassed{};

		int m_CurrentFrame{};
		int m_MaxFrames{};
	};
}