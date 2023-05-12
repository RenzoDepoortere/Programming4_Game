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
#include "GridComponent.h"

#include "PauseCommand.h"
#include "MoveCommand.h"

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


	//// Pause Button
	//// ************

	//// Create gameObject
	//std::shared_ptr<dae::GameObject> pPauseObject{ std::make_shared<dae::GameObject>() };

	//// Add components
	//// --------------

	//// Add text
	//auto pFont{ dae::ResourceManager::GetInstance().LoadFont("Other/Lingua.otf", 36) };
	//dae::TextComponent* pTextComponent{ pPauseObject->AddComponent<dae::TextComponent>() };
	//pTextComponent->SetText("PAUSED");
	//pTextComponent->SetFont(pFont);

	//// Flicker component


	//// Set position
	//const float xPos{ 694 - 36 * 5.f };
	//const float yPos{ 520 / 2.f };
	//pPauseObject->SetWorldPosition(xPos, yPos, 0.f);

	//// Add to scene
	//scene.Add(pPauseObject);

	// Command
	const auto keyState{ dae::InputMapper::KeyState::Press };
	auto controllerInput{ std::make_pair(0, dae::InputManager::ControllerButton::None) };
	auto inputKeys{ std::make_pair(SDL_SCANCODE_RETURN, controllerInput) };

	std::unique_ptr<PauseCommand> pPauseCommand{ std::make_unique<PauseCommand>() };
	dae::InputMapper::GetInstance().MapInputKey(inputKeys, keyState, std::move(pPauseCommand));
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