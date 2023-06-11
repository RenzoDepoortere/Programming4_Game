#include "DigDugScene.h"

#include "Texture2D.h"
#include "GameObject.h"
#include "Scene.h"

#include "ResourceManager.h"

#include "RenderTextureComponent.h"
#include "EnemyManager.h"
#include "GridComponent.h"
#include "PlayerManager.h"

#include "DigDugSceneManager.h"

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

void digdug::DigDugScene::SetLevel(const std::string& levelPath, unsigned int /*gameMode*/)
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
		m_pPlayerManager->Reset();
		m_pEnemyManager->Reset();
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
	std::shared_ptr<dae::GameObject> pPlayers{ std::make_shared<dae::GameObject>() };

	// Add components
	// --------------

	// PlayerManager
	m_pPlayerManager = pPlayers->AddComponent<PlayerManager>();
	m_pPlayerManager->SetNrPlayers(1);
	m_pPlayerManager->SpawnPlayers();

	// Add to root
	// -----------
	pPlayers->SetParent(m_pSceneRootObject, true);
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
	m_pEnemyManager->SpawnEnemies();

	m_pEnemyManager->ControlEnemy(enemy::Fygar);

	// Add to root
	// -----------
	pEnemies->SetParent(m_pSceneRootObject, true);
}