#include "RemoveObjectCommand.h"
#include "Scene.h"

dae::RemoveObjectCommand::RemoveObjectCommand(dae::Scene* pScene, GameObject* pToDelete)
	: SceneCommand{ pScene }
	, m_pGameObject{ pToDelete }
{
}

void dae::RemoveObjectCommand::Execute(float)
{
	GetScene()->Remove(m_pGameObject);
}