#include "InGameUIScene.h"

#include "GameObject.h"
#include "Scene.h"
#include "RenderTextureComponent.h"
#include "TextComponent.h"

#include "ResourceManager.h"

#include <memory>

InGameUIScene::InGameUIScene(dae::Scene* pScene)
{
	// Create root gameObject
	auto pRoot{ std::make_shared<dae::GameObject>() };
	pRoot->SetIsActive(false);
	pRoot->SetIsHidden(true);

	m_pSceneRootObject = pRoot.get();
	pScene->Add(pRoot);

	// Init HUD
	InitLives();
}

void InGameUIScene::SetActive(bool isActive)
{
	// Set active/inactive
	m_pSceneRootObject->SetIsActive(isActive);
	m_pSceneRootObject->SetIsHidden(!isActive);

	// If is active
	if (isActive)
	{
		m_pLivesComponent->ResetLives();
	}
}

void InGameUIScene::InitLives()
{
	// Create gameObject
	std::shared_ptr<dae::GameObject> pGameObject{ std::make_shared<dae::GameObject>() };

	// Add components
	m_pLivesComponent = pGameObject->AddComponent<LivesComponent>();
	m_pLivesComponent->SetMaxNrLives(3);
	m_pLivesComponent->ResetLives();

	// Add to root
	pGameObject->SetParent(m_pSceneRootObject, false);
}