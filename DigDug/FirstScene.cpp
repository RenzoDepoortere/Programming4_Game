#include "FirstScene.h"

#include "Font.h"
#include "Texture2D.h"
#include "GameObject.h"
#include "Scene.h"

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

#include "ServiceLocator.h"
#include "SDLSoundSystem.h"
#include "LoggingSoundSystem.h"

#include <iostream>

FirstScene::~FirstScene()
{
	// Todo: you probably don't want to initialize/destroy this for every scene
	dae::ServiceLocator::Shutdown();
}

void FirstScene::CreateGameObjects(dae::Scene& scene)
{
	BaseObjects(scene);

	Map(scene);

	MainCharacter(scene);
	Enemies(scene);
}

void FirstScene::BaseObjects(dae::Scene& /*scene*/)
{
	// Service Locator
	// ***************

#ifdef _DEBUG
	dae::ServiceLocator::RegisterSoundSystem(new dae::LoggingSoundSystem{ new dae::SDLSoundSystem{} });
#else
	dae::ServiceLocator::RegisterSoundSystem(new dae::SDLSoundSystem{});
#endif // DEBUG
}

void FirstScene::Map(dae::Scene& scene)
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
	pGridComponent->SetLevelFile("Tiles/Level1_Map.tmj");

	// Add to scene
	// ------------
	scene.Add(pGrid);
	m_pGrid = pGridComponent;
}

void FirstScene::MainCharacter(dae::Scene& scene)
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
	pCharacterComponent->SetGrid(m_pGrid);
	pCharacterComponent->SetAnimationComponent(pObjectTexture);

	// Add to scene
	// ------------
	m_pCharacters.emplace_back(pCharacterComponent);
	scene.Add(pMainCharacter);

	// Transform
	// ---------
	grid::Cell* pCell{ m_pGrid->GetCell(0) };
	pMainCharacter->SetWorldPosition(pCell->centerPosition);
}

void FirstScene::Enemies(dae::Scene& scene)
{
	// -------- Enemies --------
	// Enemies spawnData by Grid
	// *************************

	// Create gameObject
	std::shared_ptr<dae::GameObject> pEnemies{ std::make_shared<dae::GameObject>() };

	// Add components
	// --------------

	// Enemy Manager
	EnemyManager* pEnemyManager{ pEnemies->AddComponent<EnemyManager>() };
	pEnemyManager->SetGrid(m_pGrid);
	pEnemyManager->SetCharacters(m_pCharacters);
	pEnemyManager->SpawnEnemies();

	//auto controllerID = dae::InputManager::GetInstance().AddController();
	//pEnemyManager->ControlEnemy(controllerID, enemy::Pooka);

	// Add to scene
	// ------------
	scene.Add(pEnemies);
}