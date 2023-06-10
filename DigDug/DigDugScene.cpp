#include "DigDugScene.h"

#include "Font.h"
#include "Texture2D.h"
#include "GameObject.h"
#include "Scene.h"
#include "DigDugSceneManager.h"

#include "ResourceManager.h"
#include "InputManager.h"
#include "InputMapper.h"

#include "TextComponent.h"
#include "RenderTextureComponent.h"
#include "AnimationComponent.h"
#include "EnemyManager.h"
#include "GridComponent.h"
#include "CharacterComponent.h"

#include "PauseCommand.h"

#include <iostream>

digdug::DigDugScene::DigDugScene(dae::Scene* pScene)
{
	// Create root gameObject
	auto pRoot{ std::make_shared<dae::GameObject>() };	
	pRoot->SetIsActive(false);
	pRoot->SetIsHidden(true);

	m_pSceneRootObject = pRoot.get();
	pScene->Add(pRoot);
}

void digdug::DigDugScene::SetLevel(const std::string& levelPath)
{
	// Set level name
	m_LevelName = levelPath;

	// Set active
	SetActive(true);

	// Reset level
	Reset(true);
}
void digdug::DigDugScene::SetActive(bool isActive)
{
	// Set active/inactive
	m_pSceneRootObject->SetIsActive(isActive);
	m_pSceneRootObject->SetIsHidden(!isActive);
}
void digdug::DigDugScene::Reset(bool deleteObjects)
{
	// If want to delete
	if (deleteObjects)
	{
		// Delete all objects
		m_pSceneRootObject->RemoveAllChildren();

		// Re-Init objects
		Grid();
		MainCharacter();
		Enemies();
	}
	// Else
	else
	{
		// Call Reset on main components
		m_pGrid->Reset();
	}
}

void digdug::DigDugScene::Grid()
{
	// ---- Grid -----
	// Contains: Rocks
	// ***************

	// Create gameObject
	std::shared_ptr<dae::GameObject> pGrid{ std::make_shared<dae::GameObject>() };

	// Add components
	// --------------

	// Textures
	std::string textureString{ "Tiles/TileSet_Sheet.png" };
	std::shared_ptr<dae::Texture2D> pTexture{ dae::ResourceManager::GetInstance().LoadTexture(textureString) };
	dae::RenderTextureComponent* pObjectTexture{ pGrid->AddComponent<dae::RenderTextureComponent>() };
	pObjectTexture->SetTexture(pTexture);
	pObjectTexture->SetManualRender(true);

	// Grid
	auto pGridComponent{ pGrid->AddComponent<grid::GridComponent>() };
	pGridComponent->SetRockTexture("Sprites/Other/Rock_Animation.png");
	pGridComponent->SetLevelFile(m_LevelName);

	// Add to root
	// ------------
	m_pGrid = pGridComponent;
	pGrid->SetParent(m_pSceneRootObject, true);
}
void digdug::DigDugScene::MainCharacter()
{
	// Create gameObject
	// -----------------
	std::shared_ptr<dae::GameObject> pMainCharacter{ std::make_shared<dae::GameObject>() };

	// Add texture
	// -----------	
	dae::AnimationComponent* pObjectTexture{ pMainCharacter->AddComponent<dae::AnimationComponent>() };
	pObjectTexture->CenterTexture(true);

	pObjectTexture->SetSingleSpriteSize(25.f);
	pObjectTexture->SetMaxFrames(2);
	pObjectTexture->SetFramesPerSecond(12);

	pObjectTexture->SetPaused(true);

	// Add characterComponent
	// ----------------------
	CharacterComponent* pCharacterComponent{ pMainCharacter->AddComponent<CharacterComponent>() };
	pCharacterComponent->SetAnimationComponent(pObjectTexture);

	// Add to root
	// -----------
	m_pCharacters.emplace_back(pCharacterComponent);
	pMainCharacter->SetParent(m_pSceneRootObject, true);

	// Transform
	// ---------
	grid::Cell* pCell{ m_pGrid->GetCell(0) };
	pMainCharacter->SetWorldPosition(pCell->centerPosition);
}

void digdug::DigDugScene::Enemies()
{
	// -------- Enemies --------
	// Enemies spawnData by Grid
	// *************************

	// Create gameObject
	std::shared_ptr<dae::GameObject> pEnemies{ std::make_shared<dae::GameObject>() };

	// Add components
	// --------------

	// Enemy Manager
	m_pEnemyManager = pEnemies->AddComponent<EnemyManager>();
	m_pEnemyManager->SetCharacters(m_pCharacters);
	m_pEnemyManager->SpawnEnemies();

	//auto controllerID = dae::InputManager::GetInstance().AddController();
	//pEnemyManager->ControlEnemy(controllerID, enemy::Pooka);

	// Add to root
	// -----------
	pEnemies->SetParent(m_pSceneRootObject, true);
}