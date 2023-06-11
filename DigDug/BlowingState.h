#pragma once
#include "CharacterState.h"
#include "Texture2D.h"
#include "Observer.h"

#include <memory>

namespace dae
{
	class AnimationComponent;
}

namespace player
{
	class BlowingState final : public CharacterState, dae::Observer<float>
	{
	public:
		// Rule of Five
		BlowingState();
		virtual ~BlowingState() override;

		BlowingState(const BlowingState& other) = delete;
		BlowingState(BlowingState&& other) = delete;
		BlowingState& operator=(const BlowingState& other) = delete;
		BlowingState& operator=(BlowingState&& other) = delete;

		// Functionality
		virtual void OnEnter(CharacterComponent* pPlayer) override;
		virtual void OnLeave(CharacterComponent* pPlayer) override;

		virtual PlayerStates Update(CharacterComponent* pPlayer, float deltaTime) override;

		// Observer
		virtual void HandleEvent(unsigned int eventID, float deltaTime) override;
		virtual void OnSubjectDestroy() override;

	private:
		// Member variables
		// ----------------
		std::shared_ptr<dae::Texture2D> m_pBlowingSprite{ nullptr };
		dae::AnimationComponent* m_pAnimationComponent{ nullptr };
		bool m_PumpSprite{ false };

		float m_CurrentTime{};
		bool m_CanBlow{ false };

		CharacterComponent* m_pCharacterComponent{ nullptr };
		bool m_WantsToDig{ false };

		// Member functions
		// ----------------
		PlayerStates CheckEnemy(CharacterComponent* pPlayer);
		void HandleCooldown(float deltaTime);

		void Blow();
		void Unsubscribe(CharacterComponent* pPlayer);
	};
}