#pragma once
#include <vector>

namespace event
{
	enum eventID
	{
		StartMenu,
		SelectMenu,
		
		RockBreak,
		NewHighScore,
		
		EnemyAttack,
		EnemyPopped,
		EnemyDeath,
		FinalEnemy,

		PlayerWalking,
		PlayerShoot,
		PlayerBlow,
		PlayerHit,
		PlayerDeath,

		KeyboardLeft,
		KeyboardRight,
		KeyboardUp,
		KeyboardDown,
		KeyboardActionA,
		KeyboardActionB,

		ControllerLeft_1,
		ControllerRight_1,
		ControllerUp_1,
		ControllerDown_1,
		ControllerActionA_1,
		ControllerActionB_1,

		ControllerLeft_2,
		ControllerRight_2,
		ControllerUp_2,
		ControllerDown_2,
		ControllerActionA_2,
		ControllerActionB_2
	};
}