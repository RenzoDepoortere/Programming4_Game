#include "InGameUIScene.h"

#include "GameObject.h"
#include "Scene.h"
#include "RenderTextureComponent.h"
#include "TextComponent.h"
#include "ScoreComponent.h"
#include "HighScoreComponent.h"

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
	InitScore();
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

void InGameUIScene::InitScore()
{
	// Highscore
	// ---------

	// Text
	// ****
	
	// Create gameObject
	std::shared_ptr<dae::GameObject> pGameObject{ std::make_shared<dae::GameObject>() };

	// Add components
	auto font = dae::ResourceManager::GetInstance().LoadFont("Other/Arcade.ttf", 44);
	auto pTexture = std::make_shared<dae::Texture2D>(nullptr);

	auto pTextComponent = pGameObject->AddComponent<dae::TextComponent>();
	pTextComponent->SetText("HighScore");
	pTextComponent->SetFont(font);
	pTextComponent->SetTexture(pTexture);
	pTextComponent->SetColor(SDL_Color{ 220, 0, 0, 0 });

	// Add to root
	pGameObject->SetParent(m_pSceneRootObject, false);

	// Set position
	pGameObject->SetWorldPosition(500.f, 40.f, 0.f);

	// Changing Object
	// ***************

	// Create gameObject
	pGameObject = std::make_shared<dae::GameObject>();

	// Add components
	auto pHighScoreComponent{ pGameObject->AddComponent<HighScoreComponent>() };

	// Add to root
	pGameObject->SetParent(m_pSceneRootObject, false);

	// Set position
	pGameObject->SetWorldPosition(560.f, 60.f, 0.f);


	// Current score
	// -------------

	// Create gameObject
	pGameObject = std::make_shared<dae::GameObject>();

	// Add components
	m_pScoreComponent = pGameObject->AddComponent<ScoreComponent>();
	m_pScoreComponent->SetHighestScore(pHighScoreComponent->GetHighestScore());

	// Add to root
	pGameObject->SetParent(m_pSceneRootObject, false);

	// Set position
	pGameObject->SetWorldPosition(560.f, 120.f, 0.f);
}
void InGameUIScene::InitLives()
{
	// Create gameObject
	std::shared_ptr<dae::GameObject> pGameObject{ std::make_shared<dae::GameObject>() };

	// Add components
	m_pLivesComponent = pGameObject->AddComponent<LivesComponent>();
	m_pLivesComponent->SetMaxNrLives(4);
	m_pLivesComponent->ResetLives();

	// Add to root
	pGameObject->SetParent(m_pSceneRootObject, false);
}