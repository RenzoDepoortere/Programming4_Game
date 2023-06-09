#pragma once
#include "CharacterState.h"
#include "Texture2D.h"

#include <memory>

namespace player
{
	class PlayerSquashedState final : public CharacterState
	{
	public:
		// Rule of Five
		PlayerSquashedState();
		virtual ~PlayerSquashedState() override = default;

		PlayerSquashedState(const PlayerSquashedState& other) = delete;
		PlayerSquashedState(PlayerSquashedState&& other) = delete;
		PlayerSquashedState& operator=(const PlayerSquashedState& other) = delete;
		PlayerSquashedState& operator=(PlayerSquashedState&& other) = delete;

		// Functionality
		virtual void OnEnter(CharacterComponent* pPlayer) override;
		virtual void OnLeave(CharacterComponent* pPlayer) override;

		virtual PlayerStates Update(CharacterComponent* pPlayer, float deltaTime) override;

	private:
		// Member variables
		// ----------------
		std::shared_ptr<dae::Texture2D> m_pSquashedTexture{ nullptr };
	};
}