#pragma once
#include "CharacterState.h"
#include "Texture2D.h"

#include <memory>

namespace dae
{
	class AnimationComponent;
}

namespace player
{
	class HitState final : public CharacterState
	{
	public:
		// Rule of Five
		HitState();
		virtual ~HitState() override = default;

		HitState(const HitState& other) = delete;
		HitState(HitState&& other) = delete;
		HitState& operator=(const HitState& other) = delete;
		HitState& operator=(HitState&& other) = delete;

		// Functionality
		virtual void OnEnter(CharacterComponent* pPlayer) override;
		virtual void OnLeave(CharacterComponent* pPlayer) override;

		virtual PlayerStates Update(CharacterComponent* pPlayer, float deltaTime) override;

	private:
		// Member variables
		// ----------------
		std::shared_ptr<dae::Texture2D> m_pHitTexture{ nullptr };
		dae::AnimationComponent* m_pAnimationComponent{ nullptr };
	};
}