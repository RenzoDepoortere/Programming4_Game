#include "UIScene.h"

#include "GameObject.h"
#include "Scene.h"
#include "RenderTextureComponent.h"
#include "TextComponent.h"
#include "SelectionComponent.h"

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
}

void UIScene::SetActive(bool isActive)
{
	// Set active/inactive
	m_pSceneRootObject->SetIsActive(isActive);
	m_pSceneRootObject->SetIsHidden(!isActive);
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
	
	// Create gameObject
	pGameObject = std::make_shared<dae::GameObject>();

	// Add components
	auto font = dae::ResourceManager::GetInstance().LoadFont("Other/Arcade.ttf", 48);
	pTexture = std::make_shared<dae::Texture2D>(nullptr);

	dae::TextComponent* pTextComponent{ pGameObject->AddComponent<dae::TextComponent>() };
	pTextComponent->SetText("1 PLAYER");
	pTextComponent->SetFont(font);
	pTextComponent->SetTexture(pTexture);

	// Add to root
	pGameObject->SetParent(m_pSceneRootObject, false);

	// Set Position
	const float yPos{ 260 };
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

	auto pSelectionComponent = pGameObject->AddComponent<SelectionComponent>();
	pSelectionComponent->SetPositions(m_ButtonYPos);

	// Add to root
	pGameObject->SetParent(m_pSceneRootObject, false);

	// Set positions
	pGameObject->SetWorldPosition({ 180, m_ButtonYPos[0] });
}