#pragma once
class EnemyComponent;

namespace enemy
{
	enum EnemyStates
	{
		Roaming,
		Ghost,
		Chase,
		Attack,
		Caught,
		Squashed,
		Flee,
		NR_STATES
	};

	class EnemyState
	{
	public:
		// Rule of five
		EnemyState() = default;
		virtual ~EnemyState() = default;

		EnemyState(const EnemyState& other) = delete;
		EnemyState(EnemyState&& other) = delete;
		EnemyState& operator=(const EnemyState& other) = delete;
		EnemyState& operator=(EnemyState&& other) = delete;

		// Functionality
		virtual void OnEnter(EnemyComponent* pEnemy) = 0;
		virtual void OnLeave(EnemyComponent* pEnemy) = 0;

		virtual EnemyStates Update(EnemyComponent* pEnemy, float deltaTime) = 0;
	};
}