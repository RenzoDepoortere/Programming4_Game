#pragma once
#include "EnemyState.h"
#include "Texture2D.h"

#include <memory>

namespace enemy
{
	class EnemySquashedState final : public EnemyState
	{
	public:
		// Rule of Five
		EnemySquashedState();
		virtual ~EnemySquashedState() override = default;

		EnemySquashedState(const EnemySquashedState& other) = delete;
		EnemySquashedState(EnemySquashedState&& other) = delete;
		EnemySquashedState& operator=(const EnemySquashedState& other) = delete;
		EnemySquashedState& operator=(EnemySquashedState&& other) = delete;

		// Functionality
		virtual void OnEnter(EnemyComponent* pEnemy) override;
		virtual void OnLeave(EnemyComponent* pEnemy) override;

		virtual EnemyStates Update(EnemyComponent* pEnemy, float deltaTime) override;

	private:
		// Member variables
		// ----------------
		std::shared_ptr<dae::Texture2D> m_pPookaSquashedTexture{ nullptr };
		std::shared_ptr<dae::Texture2D> m_pFygarSquashedTexture{ nullptr };		
	};
}