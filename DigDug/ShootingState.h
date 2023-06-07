#pragma once
#include "CharacterState.h"
#include "Texture2D.h"

#include <memory>

namespace Player
{
	class ShootingState final : public CharacterState
	{
	public:
		// Rule of Five
		ShootingState();
		virtual ~ShootingState() override = default;

		ShootingState(const ShootingState& other) = delete;
		ShootingState(ShootingState&& other) = delete;
		ShootingState& operator=(const ShootingState& other) = delete;
		ShootingState& operator=(ShootingState&& other) = delete;

		// Functionality
		virtual void OnEnter(CharacterComponent* pPlayer) override;
		virtual void OnLeave(CharacterComponent* pPlayer) override;

		virtual PlayerStates Update(CharacterComponent* pPlayer, float deltaTime) override;

	private:
		// Member variables
		// ----------------
		std::shared_ptr<dae::Texture2D> m_pShootingSprite{ nullptr };
		std::shared_ptr<dae::Texture2D> m_pRopeSprite{ nullptr };


		// Member functions
		// ----------------
	
	};
}