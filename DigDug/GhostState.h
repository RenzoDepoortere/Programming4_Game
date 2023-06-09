#pragma once
#include "EnemyState.h"
#include "Texture2D.h"
#include "MoveCommand.h"

#include <memory>

namespace dae
{
	class AnimationComponent;
}

namespace enemy
{
	class GhostState final : public EnemyState
	{
	public:
		// Rule of Five
		GhostState();
		virtual ~GhostState() override = default;

		GhostState(const GhostState& other) = delete;
		GhostState(GhostState&& other) = delete;
		GhostState& operator=(const GhostState& other) = delete;
		GhostState& operator=(GhostState&& other) = delete;

		// Functionality
		virtual void OnEnter(EnemyComponent* pEnemy) override;
		virtual void OnLeave(EnemyComponent* pEnemy) override;

		virtual EnemyStates Update(EnemyComponent* pEnemy, float deltaTime) override;

	private:
		// Member variables
		// ----------------
		std::shared_ptr<dae::Texture2D> m_pPookaGhostTexture{ nullptr };
		std::shared_ptr<dae::Texture2D> m_pFygarGhostTexture{ nullptr };

		std::unique_ptr<dae::MoveCommand> m_pMoveCommand{ nullptr };

		// Member functions
		// ----------------
		
	};
}