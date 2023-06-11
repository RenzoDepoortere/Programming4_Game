#include "UIScene.h"

#include "GameObject.h"
#include "Scene.h"
#include "RenderTextureComponent.h"
#include "TextComponent.h"
#include "SelectionComponent.h"
#include "ScoreInputComponent.h"

#include "DigDugSceneManager.h"

#include "ResourceManager.h"

#include <memory>

UIScene::UIScene(dae::Scene* pScene)
{
	// Create root gameObject
	auto pRoot{ std::make_shared<dae::GameObject>() };
	m_pSceneRootObject = pRoot.get();
	pScene->Add(pRoot);

	// Init widgets
	InitWidgets();
	InitArrow();
	InitScoreScreen();
}

void UIScene::SetActive(bool isActive)
{
	// Set active/inactive
	m_pSceneRootObject->SetIsActive(isActive);
	m_pSceneRootObject->SetIsHidden(!isActive);
}
void UIScene::ShowScoreScreen(bool showScreen)
{
	if (showScreen)
	{
		m_pSelectionComponent->GetGameObject()->SetIsActive(false);
		m_pSelectionComponent->GetGameObject()->SetIsHidden(true);

		m_pScoreInputComponent->GetGameObject()->SetIsActive(true);
		m_pScoreInputComponent->GetGameObject()->SetIsHidden(false);
	}
	else
	{
		m_pSelectionComponent->GetGameObject()->SetIsActive(true);
		m_pSelectionComponent->GetGameObject()->SetIsHidden(false);

		m_pScoreInputComponent->GetGameObject()->SetIsActive(false);
		m_pScoreInputComponent->GetGameObject()->SetIsHidden(true);
	}
}

void UIScene::InitWidgets()
{
	// Title
	// -----

	// Create gameObject
	std::shared_ptr<dae::GameObject> pGameObject{ std::make_shared<dae::GameObject>() };

	// Add components
	auto pTexture = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Menu/TitleCutout.png");
	auto pTextureComponent = pGameObject->AddComponent<dae::RenderTextureComponent>();
	pTextureComponent->SetTexture(pTexture);

	// Add to root
	pGameObject->SetParent(m_pSceneRootObject, false);

	// Set Position
	pGameObject->SetWorldPosition(216, 90, 0);

	// Text
	// ----
	
	// 1 Player
	// ********

	// Create gameObject
	pGameObject = std::make_shared<dae::GameObject>();

	// Add components
	auto font = dae::ResourceManager::GetInstance().LoadFont("Other/Arcade.ttf", 48);
	pTexture = std::make_shared<dae::Texture2D>(nullptr);

	dae::TextComponent* pTextComponent{ pGameObject->AddComponent<dae::TextComponent>() };
	pTextComponent->SetText("SINGLE PLAYER");
	pTextComponent->SetFont(font);
	pTextComponent->SetTexture(pTexture);

	// Add to root
	pGameObject->SetParent(m_pSceneRootObject, false);

	// Set Position
	float yPos{ 260 };
	pGameObject->SetWorldPosition(260, yPos, 0);
	m_ButtonYPos.emplace_back(yPos);


	// 2 Players: PvE
	// **************
	const float distBetween{ 60 };

	// Create gameObject
	pGameObject = std::make_shared<dae::GameObject>();

	// Add components
	pTexture = std::make_shared<dae::Texture2D>(nullptr);

	pTextComponent = pGameObject->AddComponent<dae::TextComponent>();
	pTextComponent->SetText("CO OP");
	pTextComponent->SetFont(font);
	pTextComponent->SetTexture(pTexture);

	// Add to root
	pGameObject->SetParent(m_pSceneRootObject, false);

	// Set Position
	yPos += distBetween;
	pGameObject->SetWorldPosition(260, yPos, 0);
	m_ButtonYPos.emplace_back(yPos);


	// 2 Players: PvP
	// **************

	// Create gameObject
	pGameObject = std::make_shared<dae::GameObject>();

	// Add components
	pTexture = std::make_shared<dae::Texture2D>(nullptr);

	pTextComponent = pGameObject->AddComponent<dae::TextComponent>();
	pTextComponent->SetText("VERSUS");
	pTextComponent->SetFont(font);
	pTextComponent->SetTexture(pTexture);

	// Add to root
	pGameObject->SetParent(m_pSceneRootObject, false);

	// Set Position
	yPos += distBetween;
	pGameObject->SetWorldPosition(260, yPos, 0);
	m_ButtonYPos.emplace_back(yPos);

}
void UIScene::InitArrow()
{
	// Arrow
	// -----

	// Create gameObject
	std::shared_ptr<dae::GameObject> pGameObject{ std::make_shared<dae::GameObject>() };

	// Add components
	auto pTexture = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Menu/Arrow.png");
	auto pTextureComponent = pGameObject->AddComponent<dae::RenderTextureComponent>();
	pTextureComponent->SetTexture(pTexture);

	m_pSelectionComponent = pGameObject->AddComponent<SelectionComponent>();
	m_pSelectionComponent->SetPositions(m_ButtonYPos);

	const float xPos{ 180.f };
	m_pSelectionComponent->SetXPos(xPos);

	auto singlePlayer = []()
	{
		digdug::DigDugSceneManager::GetInstance().SetGameMode(digdug::Single);
		digdug::DigDugSceneManager::GetInstance().NextLevel();
	};
	auto coop = []()
	{
		digdug::DigDugSceneManager::GetInstance().SetGameMode(digdug::PvE);
		digdug::DigDugSceneManager::GetInstance().NextLevel();
	};
	auto versus = []()
	{
		digdug::DigDugSceneManager::GetInstance().SetGameMode(digdug::PvP);
		digdug::DigDugSceneManager::GetInstance().NextLevel();
	};
	std::vector<std::function<void()>> functions{};
	functions.emplace_back(singlePlayer);
	functions.emplace_back(coop);
	functions.emplace_back(versus);

	m_pSelectionComponent->SetActivateFunctions(functions);

	// Add to root
	pGameObject->SetParent(m_pSceneRootObject, false);

	// Set pos
	pGameObject->SetWorldPosition({ xPos, m_ButtonYPos[0] });
}
void UIScene::InitScoreScreen()
{
	// Create gameObject
	std::shared_ptr<dae::GameObject> pGameObject{ std::make_shared<dae::GameObject>() };

	// Add components
	m_pScoreInputComponent = pGameObject->AddComponent<ScoreInputComponent>();

	// Add to root
	pGameObject->SetParent(m_pSceneRootObject, false);
	pGameObject->SetIsActive(false);
	pGameObject->SetIsHidden(true);

	// Set Position
	pGameObject->SetWorldPosition(216, 90, 0);
}