#include "SkipLevelCommand.h"

#include "DigDugSceneManager.h"

void SkipLevelCommand::Execute(float /*deltaTime*/)
{
	digdug::DigDugSceneManager::GetInstance().NextLevel();
}