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
	class BlowingState final : public CharacterState
	{
	public:
		// Rule of Five
		BlowingState();
		virtual ~BlowingState() override = default;

		BlowingState(const BlowingState& other) = delete;
		BlowingState(BlowingState&& other) = delete;
		BlowingState& operator=(const BlowingState& other) = delete;
		BlowingState& operator=(BlowingState&& other) = delete;

		// Functionality
		virtual void OnEnter(CharacterComponent* pPlayer) override;
		virtual void OnLeave(CharacterComponent* pPlayer) override;

		virtual PlayerStates Update(CharacterComponent* pPlayer, float deltaTime) override;

	private:
		// Member variables
		// ----------------
		std::shared_ptr<dae::Texture2D> m_pBlowingSprite{ nullptr };
		dae::AnimationComponent* m_pAnimationComponent{ nullptr };

		bool m_PumpSprite{ false };

		// Member functions
		// ----------------
		PlayerStates CheckEnemy(CharacterComponent* pPlayer);
		PlayerStates HandleInput(CharacterComponent* pPlayer);
	};
}