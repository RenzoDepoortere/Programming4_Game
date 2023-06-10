#pragma once
#include <vector>

namespace event
{
	enum eventID
	{
		PauseMenu,
		
		RockBreak,
		
		EnemyAttack,
		EnemyPopped,
		EnemyDeath,
		FinalEnemy,

		PlayerWalking,
		PlayerShoot,
		PlayerBlow,
		PlayerHit,
		PlayerDeath
	};
}