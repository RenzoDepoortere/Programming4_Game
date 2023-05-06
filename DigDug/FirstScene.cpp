#include "FirstScene.h"

#include "Texture2D.h"
#include "GameObject.h"
#include "Scene.h"

#include "ResourceManager.h"
#include "InputManager.h"
#include "InputMapper.h"

#include "RenderTextureComponent.h"
#include "GridComponent.h"

#include "MoveCommand.h"

#include <iostream>

void FirstScene::CreateGameObjects(dae::Scene& scene)
{
	Map(scene);
	MainCharacter(scene);
}

void FirstScene::Map(dae::Scene& scene)
{
	// Grid
	// ****

	// Create gameObject
	std::shared_ptr<dae::GameObject> pGrid{ std::make_shared<dae::GameObject>() };
	
	// Add components
	// --------------

	// Textures
	std::string textureString{ "Tiles/TileSet_Sheet.png" };
	std::shared_ptr<dae::Texture2D> pTexture{ dae::ResourceManager::GetInstance().LoadTexture(textureString) };
	dae::RenderTextureComponent* pObjectTexture{ pGrid->AddComponent<dae::RenderTextureComponent>() };
	pObjectTexture->CenterTexture(true);
	pObjectTexture->SetTexture(pTexture);
	pObjectTexture->SetManualRender(true);

	// Grid
	auto pGridComponent{ pGrid->AddComponent<grid::GridComponent>() };
	pGridComponent->SetRockTexture("Sprites/Single_Rock.png");
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
	std::string textureString{ "Sprites/MC.png" };
	std::shared_ptr<dae::Texture2D> pTexture{ dae::ResourceManager::GetInstance().LoadTexture(textureString) };
	dae::RenderTextureComponent* pObjectTexture{ pMainCharacter->AddComponent<dae::RenderTextureComponent>() };
	pObjectTexture->CenterTexture(true);
	pObjectTexture->SetTexture(pTexture);

	// Add movement
	// ------------
	float movementSpeed{ 100.f };
	glm::vec2 movementDirection{};
	const auto keyState{ dae::InputMapper::KeyState::Hold };

	// LEFT
	auto controllerInput{ std::make_pair(0, dae::InputManager::ControllerButton::None) };
	auto inputKeys{ std::make_pair(SDL_SCANCODE_A, controllerInput) };

	movementDirection = glm::vec2{ -1, 0 };

	std::unique_ptr<dae::MoveCommand> pMoveCommand{ std::make_unique<dae::MoveCommand>(pMainCharacter.get(), movementDirection, movementSpeed, m_pGrid) };
	dae::InputMapper::GetInstance().MapInputKey(inputKeys, keyState, std::move(pMoveCommand));


	// RIGHT
	controllerInput = std::make_pair(0, dae::InputManager::ControllerButton::None);
	inputKeys = std::make_pair(SDL_SCANCODE_D, controllerInput);

	movementDirection = glm::vec2{ 1, 0 };

	pMoveCommand = std::make_unique<dae::MoveCommand>(pMainCharacter.get(), movementDirection, movementSpeed, m_pGrid);
	dae::InputMapper::GetInstance().MapInputKey(inputKeys, keyState, std::move(pMoveCommand));


	// DOWN
	controllerInput = std::make_pair(0, dae::InputManager::ControllerButton::None);
	inputKeys = std::make_pair(SDL_SCANCODE_S, controllerInput);

	movementDirection = glm::vec2{ 0, 1 };

	pMoveCommand = std::make_unique<dae::MoveCommand>(pMainCharacter.get(), movementDirection, movementSpeed, m_pGrid);
	dae::InputMapper::GetInstance().MapInputKey(inputKeys, keyState, std::move(pMoveCommand));


	// UP
	controllerInput = std::make_pair(0, dae::InputManager::ControllerButton::None);
	inputKeys = std::make_pair(SDL_SCANCODE_W, controllerInput);

	movementDirection = glm::vec2{ 0, -1 };

	pMoveCommand = std::make_unique<dae::MoveCommand>(pMainCharacter.get(), movementDirection, movementSpeed, m_pGrid);
	dae::InputMapper::GetInstance().MapInputKey(inputKeys, keyState, std::move(pMoveCommand));

	// Add to scene
	// ------------
	scene.Add(pMainCharacter);
}