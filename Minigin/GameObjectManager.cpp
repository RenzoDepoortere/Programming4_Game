#include <memory>
#include <SDL.h>

#include "GameObjectManager.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Utils.h"
#include "InputMapper.h"

#include "Texture2D.h"
#include "RenderTextureComponent.h"
#include "TextComponent.h"
#include "FPSCounter.h"
#include "RotatingMovement.h"
#include "TrashTheCache.h"
#include "MoveCommand.h"
#include "HealthComponent.h"
#include "KillCommand.h"
#include "PointCommand.h"
#include "RemoveObjectCommand.h"
#include "PointComponent.h"

void dae::GameObjectManager::CreateGameObjects(Scene& scene)
{
	FirstWeekObjects(scene);
	//SecondWeekObjects(scene);
	//FourthWeekObjects(scene);
	FifthWeekObjects(scene);
	SixthWeekObjects(scene);
}

void dae::GameObjectManager::FirstWeekObjects(Scene& scene)
{
	// Background
	// ----------

	// Create gameObject
	auto go = std::make_shared<dae::GameObject>();

	// Add component
	std::shared_ptr<Texture2D> pTexture{ dae::ResourceManager::GetInstance().LoadTexture("background.tga") };

	dae::RenderTextureComponent* pTextureComponent{ go->AddComponent<dae::RenderTextureComponent>() };
	pTextureComponent->SetTexture(pTexture);

	// Add to Scene
	scene.Add(go);

	// Logo
	// ----

	// Create gameObject
	go = std::make_shared<dae::GameObject>();

	// Add component
	pTexture = dae::ResourceManager::GetInstance().LoadTexture("logo.tga");

	pTextureComponent = go->AddComponent<dae::RenderTextureComponent>();
	pTextureComponent->SetTexture(pTexture);

	// Set Position
	go->SetWorldPosition(216, 180, 0);

	// Add to Scene
	scene.Add(go);


	// Text
	// ---

	// Create font
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	// Create gameObject
	go = std::make_shared<dae::GameObject>();

	// Add components
	pTexture = std::make_shared<dae::Texture2D>(nullptr);

	dae::TextComponent* pTextComponent{ go->AddComponent<dae::TextComponent>() };
	pTextComponent->SetText("Programming 4 Assignment");
	pTextComponent->SetFont(font);
	pTextComponent->SetTexture(pTexture);

	// Set Position
	go->SetWorldPosition(80, 20, 0);

	// Add to Scene
	scene.Add(go);


	// FPS-Counter
	// -----------

	// Create font
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);

	// Create gameObject
	go = std::make_shared<dae::GameObject>();
	pTexture = std::make_shared<dae::Texture2D>(nullptr);

	// Add components
	pTextComponent = go->AddComponent<dae::TextComponent>();
	pTextComponent->SetFont(font);
	pTextComponent->SetColor(SDL_Color{ 0,255,0 });
	pTextComponent->SetTexture(pTexture);

	go->AddComponent<dae::FPSCounter>();

	// Add to Scene
	scene.Add(go);
}
void dae::GameObjectManager::SecondWeekObjects(Scene& scene)
{
	const float rotationDistance{ 50.f };

	// First Rotation Point
	// --------------------
	const glm::vec3 startPos{ 320.f, 350.f, 0.f };

	// create gameObject
	std::shared_ptr<dae::GameObject> pStartPos{ std::make_shared<dae::GameObject>() };
	pStartPos->SetWorldPosition(startPos);

	scene.Add(pStartPos);


	// Main Character Sprite
	// ---------------------
	std::string textureString{ "MC.png" };

	// Create gameObject
	std::shared_ptr<dae::GameObject> pMainCharacter{ std::make_shared<dae::GameObject>() };

	// Add components
	std::shared_ptr<dae::Texture2D> pTexture{ dae::ResourceManager::GetInstance().LoadTexture(textureString) };
	dae::RenderTextureComponent* pObjectTexture{ pMainCharacter->AddComponent<dae::RenderTextureComponent>() };
	pObjectTexture->SetTexture(pTexture);

	dae::RotatingMovement* pRotatingMovement{ pMainCharacter->AddComponent<dae::RotatingMovement>() };
	pRotatingMovement->SetRotationDistance(rotationDistance);

	pMainCharacter->SetParent(pStartPos.get(), false);

	// Enemy Character Sprite
	// ----------------------
	textureString = "Enemy.png";

	// Create gameObject
	std::shared_ptr<dae::GameObject> pEnemyCharacter{ std::make_shared<dae::GameObject>() };

	// Add components
	pTexture = dae::ResourceManager::GetInstance().LoadTexture(textureString);
	pObjectTexture = pEnemyCharacter->AddComponent<dae::RenderTextureComponent>();
	pObjectTexture->SetTexture(pTexture);

	pRotatingMovement = pEnemyCharacter->AddComponent<dae::RotatingMovement>();
	pRotatingMovement->SetRotationDistance(rotationDistance);
	pRotatingMovement->SetClockwiseRotationDirection(false);
	pRotatingMovement->SetRotationSpeed(pRotatingMovement->GetRotationSpeed() * 2.f);

	// Set ownership
	pEnemyCharacter->SetParent(pMainCharacter.get(), false);
}
void dae::GameObjectManager::FourthWeekObjects(Scene& scene)
{
	// First Exercise
	// --------------
	std::shared_ptr<GameObject> pFirstExercise{ std::make_shared<GameObject>() };
	pFirstExercise->AddComponent<dae::TrashTheCache>();

	scene.Add(pFirstExercise);
}
void dae::GameObjectManager::FifthWeekObjects(Scene& scene)
{
	float movementSpeed{ 100.f };
	const auto keyState{ dae::InputMapper::KeyState::Hold };
	glm::vec2 movementDirection{};

	const glm::vec3 startPos{ 320.f, 350.f, 0.f };

	// Main Character Sprite
	// ---------------------
	std::string textureString{ "MC.png" };

	// Create gameObject
	std::shared_ptr<dae::GameObject> pMainCharacter{ std::make_shared<dae::GameObject>() };
	pMainCharacter->SetWorldPosition(startPos);

	// Add components
	std::shared_ptr<dae::Texture2D> pTexture{ dae::ResourceManager::GetInstance().LoadTexture(textureString) };
	dae::RenderTextureComponent* pObjectTexture{ pMainCharacter->AddComponent<dae::RenderTextureComponent>() };
	pObjectTexture->SetTexture(pTexture);
	
	// Add to scene
	scene.Add(pMainCharacter);
	m_pMainCharacter = pMainCharacter.get();

	// Enemy Character Sprite
	// ----------------------
	textureString = "Enemy.png";

	// Create gameObject
	std::shared_ptr<dae::GameObject> pEnemyCharacter{ std::make_shared<dae::GameObject>() };
	pEnemyCharacter->SetWorldPosition(startPos);

	// Add components
	pTexture = dae::ResourceManager::GetInstance().LoadTexture(textureString);
	pObjectTexture = pEnemyCharacter->AddComponent<dae::RenderTextureComponent>();
	pObjectTexture->SetTexture(pTexture);

	// Add to scene
	scene.Add(pEnemyCharacter);
	m_pSideCharacter = pEnemyCharacter.get();


	// Input Mapper
	// ------------


	// Main Character
	// ************** 
	
	// LEFT
	auto controllerInput{ std::make_pair(0, dae::InputManager::ControllerButton::None) };
	auto inputKeys{ std::make_pair(SDL_SCANCODE_A, controllerInput)};

	movementDirection = glm::vec2{ -1, 0 };

	std::unique_ptr<MoveCommand> pMoveCommand{ std::make_unique<MoveCommand>(pMainCharacter.get(), movementDirection, movementSpeed) };
	dae::InputMapper::GetInstance().MapInputKey(inputKeys, keyState, std::move(pMoveCommand));


	// RIGHT
	controllerInput = std::make_pair(0, dae::InputManager::ControllerButton::None);
	inputKeys = std::make_pair(SDL_SCANCODE_D, controllerInput);

	movementDirection = glm::vec2{ 1, 0 };

	pMoveCommand = std::make_unique<MoveCommand>(pMainCharacter.get(), movementDirection, movementSpeed);
	dae::InputMapper::GetInstance().MapInputKey(inputKeys, keyState, std::move(pMoveCommand));


	// DOWN
	controllerInput = std::make_pair(0, dae::InputManager::ControllerButton::None);
	inputKeys = std::make_pair(SDL_SCANCODE_S, controllerInput);

	movementDirection = glm::vec2{ 0, 1 };

	pMoveCommand = std::make_unique<MoveCommand>(pMainCharacter.get(), movementDirection, movementSpeed);
	dae::InputMapper::GetInstance().MapInputKey(inputKeys, keyState, std::move(pMoveCommand));


	// UP
	controllerInput = std::make_pair(0, dae::InputManager::ControllerButton::None);
	inputKeys = std::make_pair(SDL_SCANCODE_W, controllerInput);

	movementDirection = glm::vec2{ 0, -1 };

	pMoveCommand = std::make_unique<MoveCommand>(pMainCharacter.get(), movementDirection, movementSpeed);
	dae::InputMapper::GetInstance().MapInputKey(inputKeys, keyState, std::move(pMoveCommand));


	// Side Character
	// ************** 

	m_ControllerIdx = dae::InputManager::GetInstance().AddController();
	movementSpeed *= 2.f;

	// LEFT
	controllerInput = std::make_pair(m_ControllerIdx, dae::InputManager::ControllerButton::DPadLeft);
	inputKeys = std::make_pair(SDL_SCANCODE_UNKNOWN, controllerInput);

	movementDirection = glm::vec2{ -1, 0 };

	pMoveCommand = std::make_unique<MoveCommand>(pEnemyCharacter.get(), movementDirection, movementSpeed);
	dae::InputMapper::GetInstance().MapInputKey(inputKeys, keyState, std::move(pMoveCommand));


	// RIGHT
	controllerInput = std::make_pair(m_ControllerIdx, dae::InputManager::ControllerButton::DPadRight);
	inputKeys = std::make_pair(SDL_SCANCODE_UNKNOWN, controllerInput);

	movementDirection = glm::vec2{ 1, 0 };

	pMoveCommand = std::make_unique<MoveCommand>(pEnemyCharacter.get(), movementDirection, movementSpeed);
	dae::InputMapper::GetInstance().MapInputKey(inputKeys, keyState, std::move(pMoveCommand));


	// DOWN
	controllerInput = std::make_pair(m_ControllerIdx, dae::InputManager::ControllerButton::DPadDown);
	inputKeys = std::make_pair(SDL_SCANCODE_UNKNOWN, controllerInput);

	movementDirection = glm::vec2{ 0, 1 };

	pMoveCommand = std::make_unique<MoveCommand>(pEnemyCharacter.get(), movementDirection, movementSpeed);
	dae::InputMapper::GetInstance().MapInputKey(inputKeys, keyState, std::move(pMoveCommand));


	// UP
	controllerInput = std::make_pair(m_ControllerIdx, dae::InputManager::ControllerButton::DPadUp);
	inputKeys = std::make_pair(SDL_SCANCODE_UNKNOWN, controllerInput);

	movementDirection = glm::vec2{ 0, -1 };

	pMoveCommand = std::make_unique<MoveCommand>(pEnemyCharacter.get(), movementDirection, movementSpeed);
	dae::InputMapper::GetInstance().MapInputKey(inputKeys, keyState, std::move(pMoveCommand));
}
void dae::GameObjectManager::SixthWeekObjects(Scene& scene)
{
	// Components
	// **********

	// Main Character
	// --------------
	m_pMainCharacter->AddComponent<dae::HealthComponent>();
	m_pMainCharacter->AddComponent<dae::PointComponent>();


	// Secondary Character
	// -------------------
	m_pSideCharacter->AddComponent<dae::HealthComponent>();
	m_pSideCharacter->AddComponent<dae::PointComponent>();


	// Observers
	// *********

	// First healthDisplayObject
	// -------------------------
	glm::vec3 startPos{ 0.f, 300.f, 0.f };

	// Create font
	std::shared_ptr<dae::Font> pFont{ dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18) };

	// Create gameObject
	std::shared_ptr<dae::GameObject> pHealthDisplayObject{ std::make_shared<dae::GameObject>() };
	std::shared_ptr<Texture2D> pTexture = std::make_shared<dae::Texture2D>(nullptr);

	pHealthDisplayObject->SetWorldPosition(startPos);

	// Add components
	dae::TextComponent* pTextComponent = pHealthDisplayObject->AddComponent<dae::TextComponent>();
	pTextComponent->SetFont(pFont);
	pTextComponent->SetColor(SDL_Color{ 250,253, 15 });
	pTextComponent->SetTexture(pTexture);
	pTextComponent->SetText("Test");

	// Add to Scene
	scene.Add(pHealthDisplayObject);


	// Second healthDisplayObject
	// --------------------------
	startPos.y += 75.f;

	// Create gameObject
	pHealthDisplayObject = std::make_shared<dae::GameObject>();
	pTexture = std::make_shared<dae::Texture2D>(nullptr);

	pHealthDisplayObject->SetWorldPosition(startPos);

	// Add components
	pTextComponent = pHealthDisplayObject->AddComponent<dae::TextComponent>();
	pTextComponent->SetFont(pFont);
	pTextComponent->SetColor(SDL_Color{ 0, 253, 0 });
	pTextComponent->SetTexture(pTexture);
	pTextComponent->SetText("Test");
	
	// Add to Scene
	scene.Add(pHealthDisplayObject);


	// First pointObject
	// -----------------
	startPos = {0.f, 325.0f, 0.f };

	// Create gameObject
	std::shared_ptr<dae::GameObject> pPointDisplayObject{ std::make_shared<dae::GameObject>() };
	pTexture = std::make_shared<dae::Texture2D>(nullptr);

	pPointDisplayObject->SetWorldPosition(startPos);

	// Add components
	pTextComponent = pPointDisplayObject->AddComponent<dae::TextComponent>();
	pTextComponent->SetFont(pFont);
	pTextComponent->SetColor(SDL_Color{ 250,253, 15 });
	pTextComponent->SetTexture(pTexture);
	pTextComponent->SetText("Test");

	// Add to Scene
	scene.Add(pPointDisplayObject);
	

	// Second pointObject
	// ------------------
	startPos.y += 75.f;

	// Create gameObject
	pPointDisplayObject = std::make_shared<dae::GameObject>();
	pTexture = std::make_shared<dae::Texture2D>(nullptr);

	pPointDisplayObject->SetWorldPosition(startPos);

	// Add components
	pTextComponent = pPointDisplayObject->AddComponent<dae::TextComponent>();
	pTextComponent->SetFont(pFont);
	pTextComponent->SetColor(SDL_Color{ 0, 253, 0 });
	pTextComponent->SetTexture(pTexture);
	pTextComponent->SetText("Test");

	// Add to Scene
	scene.Add(pPointDisplayObject);


	// Button Prompt
	// *************
	std::shared_ptr<GameObject> pButtonPrompt = std::make_shared<dae::GameObject>();

	// Add component
	pTexture = dae::ResourceManager::GetInstance().LoadTexture("Controls.png");

	dae::RenderTextureComponent* pTextureComponent{ pButtonPrompt->AddComponent<dae::RenderTextureComponent>() };
	pTextureComponent->SetTexture(pTexture);

	// Add to Scene
	scene.Add(pButtonPrompt);

	// Display
	// -------
	auto controllerInput = std::make_pair(0, dae::InputManager::ControllerButton::None);
	auto inputKeys = std::make_pair(SDL_SCANCODE_RETURN, controllerInput);

	// Display command
	std::unique_ptr<RemoveObjectCommand> pSceneCommand{ std::make_unique<RemoveObjectCommand>(&scene, pButtonPrompt.get()) };
	dae::InputMapper::GetInstance().MapInputKey(inputKeys, InputMapper::KeyState::Press, std::move(pSceneCommand));
}