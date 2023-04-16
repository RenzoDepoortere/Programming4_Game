#include "GameObjectCommand.h"
#include "GameObject.h"

dae::GameObjectCommand::GameObjectCommand(dae::GameObject* pActor)
	: m_pActor{ pActor }
{
}