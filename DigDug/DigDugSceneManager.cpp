#include "DigDugSceneManager.h"

#include "Texture2D.h"
#include "RenderTextureComponent.h"
#include "GameObject.h"
#include "GridComponent.h"
#include "EventCommand.h"
#include "SkipLevelCommand.h"
#include "MuteCommand.h"

#include "ServiceLocator.h"
#include "SDLSoundSystem.h"
#include "LoggingSoundSystem.h"
#include "ResourceManager.h"
#include "EventsEnum.h"
#include "EventManager.h"
#include "InputManager.h"
#include "InputMapper.h"

#include "Scene.h"
#include <iostream>

using namespace digdug;

DigDugSceneManager::DigDugSceneManager()
	: m_CurrentLevel{ -1 }
{
	// Subscribe to events
	dae::EventManager<>::GetInstance().Subscribe(event::PlayerDeath, this);
}
DigDugSceneManager::~DigDugSceneManager()
{
	// Unsubscribe to events
	if (dae::EventManager<>::GetIsDestroyed() == false)
	{
		dae::EventManager<>::GetInstance().Unsubscribe(event::PlayerDeath, this);
	}

	// Shutdown services
	dae::ServiceLocator::Shutdown();
}

void DigDugSceneManager::Initialize(const std::vector<dae::Scene*>& pScenes)
{
	InitSystems();

	InitInput();

	InitMenu(pScenes);
	InitMainGame(pScenes);
	InitScenes(pScenes);
}
void DigDugSceneManager::NextLevel()
{
	// Set changingLevel to true
	m_IsChangingLevel = true;

	// Go up a level
	++m_CurrentLevel;
	if (m_LevelNames.size() <= m_CurrentLevel)
	{
		GoToMenu();
		return;
	}

	// Un/Hide UI
	m_pUIScene->SetActive(false);
	m_pInGameUIScene->SetActive(true);

	// Set levelName
	m_pCurrentScene->SetLevel(m_LevelNames[m_CurrentLevel], m_CurrentGameMode);

	// Set changingLevel to false
	m_IsChangingLevel = false;
}

void DigDugSceneManager::HandleEvent(unsigned int eventID)
{
	if (eventID != event::PlayerDeath) return;

	// Check whether player completely died
	if (m_pInGameUIScene->LoseLive())
	{
		GoToMenu();
	}
	else
	{
		m_pCurrentScene->Reset(false);
	}
}
void DigDugSceneManager::OnSubjectDestroy()
{
}

void DigDugSceneManager::InitSystems()
{
	// Service Locator
	// ***************

	// Sound
//#ifdef _DEBUG
//	dae::ServiceLocator::RegisterSoundSystem(new dae::LoggingSoundSystem{ new dae::SDLSoundSystem{} });
//#else
//	dae::ServiceLocator::RegisterSoundSystem(new dae::SDLSoundSystem{});
//#endif // DEBUG

	dae::ServiceLocator::RegisterSoundSystem(new dae::SDLSoundSystem{});
}

void DigDugSceneManager::InitInput()
{
	// Add controllers
	// ---------------
	m_ControllerIndices.emplace_back(dae::InputManager::GetInstance().AddController());
	m_ControllerIndices.emplace_back(dae::InputManager::GetInstance().AddController());

	// Commands
	// --------
	KeyboardCommands();
	ControllerCommand(m_ControllerIndices[0]);
	ControllerCommand(m_ControllerIndices[1]);

	CommunalCommands();
}
void DigDugSceneManager::KeyboardCommands()
{
	auto keyState{ dae::InputMapper::KeyState::Hold };

	// LEFT
	auto controllerInput{ std::make_pair(0, dae::InputManager::ControllerButton::None) };
	auto inputKeys{ std::make_pair(SDL_SCANCODE_A, controllerInput) };

	std::unique_ptr<EventCommand> eventCommand{ std::make_unique<EventCommand>(event::KeyboardLeft) };
	dae::InputMapper::GetInstance().MapInputKey(inputKeys, keyState, std::move(eventCommand));

	// RIGHT
	controllerInput = std::make_pair(0, dae::InputManager::ControllerButton::None);
	inputKeys = std::make_pair(SDL_SCANCODE_D, controllerInput);

	eventCommand = std::make_unique<EventCommand>(event::KeyboardRight);
	dae::InputMapper::GetInstance().MapInputKey(inputKeys, keyState, std::move(eventCommand));

	// DOWN
	controllerInput = std::make_pair(0, dae::InputManager::ControllerButton::None);
	inputKeys = std::make_pair(SDL_SCANCODE_S, controllerInput);

	eventCommand = std::make_unique<EventCommand>(event::KeyboardDown);
	dae::InputMapper::GetInstance().MapInputKey(inputKeys, keyState, std::move(eventCommand));

	// UP
	controllerInput = std::make_pair(0, dae::InputManager::ControllerButton::None);
	inputKeys = std::make_pair(SDL_SCANCODE_W, controllerInput);

	eventCommand = std::make_unique<EventCommand>(event::KeyboardUp);
	dae::InputMapper::GetInstance().MapInputKey(inputKeys, keyState, std::move(eventCommand));


	keyState = dae::InputMapper::KeyState::Press;
	// ACTION A
	controllerInput = std::make_pair(0, dae::InputManager::ControllerButton::None);
	inputKeys = std::make_pair(SDL_SCANCODE_J, controllerInput);

	eventCommand = std::make_unique<EventCommand>(event::KeyboardActionA);
	dae::InputMapper::GetInstance().MapInputKey(inputKeys, keyState, std::move(eventCommand));

	// ACTION B
	controllerInput = std::make_pair(0, dae::InputManager::ControllerButton::None);
	inputKeys = std::make_pair(SDL_SCANCODE_K, controllerInput);

	eventCommand = std::make_unique<EventCommand>(event::KeyboardActionB);
	dae::InputMapper::GetInstance().MapInputKey(inputKeys, keyState, std::move(eventCommand));


	// SKIP LEVEL
	controllerInput = std::make_pair(0, dae::InputManager::ControllerButton::None);
	inputKeys = std::make_pair(SDL_SCANCODE_F1, controllerInput);

	auto pSkipLevel = std::make_unique<SkipLevelCommand>();
	dae::InputMapper::GetInstance().MapInputKey(inputKeys, keyState, std::move(pSkipLevel));

	// MUTE
	controllerInput = std::make_pair(0, dae::InputManager::ControllerButton::None);
	inputKeys = std::make_pair(SDL_SCANCODE_F2, controllerInput);

	auto pMuteCommand = std::make_unique<MuteCommand>();
	dae::InputMapper::GetInstance().MapInputKey(inputKeys, keyState, std::move(pMuteCommand));
}
void DigDugSceneManager::ControllerCommand(unsigned long controllerIdx)
{
	const int nrControllerEvents{ 6 };
	const int addition{ static_cast<int>(controllerIdx * nrControllerEvents) };

	auto keyState{ dae::InputMapper::KeyState::Hold };

	// LEFT
	auto controllerInput{ std::make_pair(controllerIdx, dae::InputManager::ControllerButton::DPadLeft) };
	auto inputKeys{ std::make_pair(SDL_SCANCODE_UNKNOWN, controllerInput) };

	std::unique_ptr<EventCommand> eventCommand{ std::make_unique<EventCommand>(event::ControllerLeft_1 + addition) };
	dae::InputMapper::GetInstance().MapInputKey(inputKeys, keyState, std::move(eventCommand));

	// RIGHT
	controllerInput = std::make_pair(controllerIdx, dae::InputManager::ControllerButton::DPadRight);
	inputKeys = std::make_pair(SDL_SCANCODE_UNKNOWN, controllerInput);

	eventCommand = std::make_unique<EventCommand>(event::ControllerRight_1 + addition);
	dae::InputMapper::GetInstance().MapInputKey(inputKeys, keyState, std::move(eventCommand));

	// DOWN
	controllerInput = std::make_pair(controllerIdx, dae::InputManager::ControllerButton::DPadDown);
	inputKeys = std::make_pair(SDL_SCANCODE_UNKNOWN, controllerInput);

	eventCommand = std::make_unique<EventCommand>(event::ControllerDown_1 + addition);
	dae::InputMapper::GetInstance().MapInputKey(inputKeys, keyState, std::move(eventCommand));

	// UP
	controllerInput = std::make_pair(controllerIdx, dae::InputManager::ControllerButton::DPadUp);
	inputKeys = std::make_pair(SDL_SCANCODE_UNKNOWN, controllerInput);

	eventCommand = std::make_unique<EventCommand>(event::ControllerUp_1 + addition);
	dae::InputMapper::GetInstance().MapInputKey(inputKeys, keyState, std::move(eventCommand));


	keyState = dae::InputMapper::KeyState::Press;
	// ACTION A
	controllerInput = std::make_pair(controllerIdx, dae::InputManager::ControllerButton::ButtonA);
	inputKeys = std::make_pair(SDL_SCANCODE_UNKNOWN, controllerInput);

	eventCommand = std::make_unique<EventCommand>(event::ControllerActionA_1 + addition);
	dae::InputMapper::GetInstance().MapInputKey(inputKeys, keyState, std::move(eventCommand));

	// ACTION B
	controllerInput = std::make_pair(controllerIdx, dae::InputManager::ControllerButton::ButtonX);
	inputKeys = std::make_pair(SDL_SCANCODE_UNKNOWN, controllerInput);

	eventCommand = std::make_unique<EventCommand>(event::ControllerActionB_1 + addition);
	dae::InputMapper::GetInstance().MapInputKey(inputKeys, keyState, std::move(eventCommand));
}
void DigDugSceneManager::CommunalCommands()
{
	auto keyState{ dae::InputMapper::KeyState::Press };

	// SELECT
	// ------

	// Keyboard
	auto controllerInput{ std::make_pair(0, dae::InputManager::ControllerButton::None) };
	auto inputKeys{ std::make_pair(SDL_SCANCODE_ESCAPE, controllerInput) };

	std::unique_ptr<EventCommand> eventCommand{ std::make_unique<EventCommand>(event::SelectMenu) };
	dae::InputMapper::GetInstance().MapInputKey(inputKeys, keyState, std::move(eventCommand));

	// Controllers
	for (int idx{}; idx < m_ControllerIndices.size(); ++idx)
	{
		controllerInput = std::make_pair(idx, dae::InputManager::ControllerButton::Select);
		inputKeys = std::make_pair(SDL_SCANCODE_UNKNOWN, controllerInput);

		eventCommand = std::make_unique<EventCommand>(event::SelectMenu);
		dae::InputMapper::GetInstance().MapInputKey(inputKeys, keyState, std::move(eventCommand));
	}

	// START
	// -----

	// Keyboard
	controllerInput = std::make_pair(0, dae::InputManager::ControllerButton::None);
	inputKeys = std::make_pair(SDL_SCANCODE_RETURN, controllerInput);

	eventCommand = std::make_unique<EventCommand>(event::StartMenu);
	dae::InputMapper::GetInstance().MapInputKey(inputKeys, keyState, std::move(eventCommand));

	// Controllers
	for (int idx{}; idx < m_ControllerIndices.size(); ++idx)
	{
		controllerInput = std::make_pair(idx, dae::InputManager::ControllerButton::Start);
		inputKeys = std::make_pair(SDL_SCANCODE_UNKNOWN, controllerInput);

		eventCommand = std::make_unique<EventCommand>(event::StartMenu);
		dae::InputMapper::GetInstance().MapInputKey(inputKeys, keyState, std::move(eventCommand));
	}
}

void DigDugSceneManager::InitMenu(const std::vector<dae::Scene*>& pScenes)
{
	// Create UI scene
	m_pUIScene = std::make_unique<UIScene>(pScenes[0]);
}
void DigDugSceneManager::InitMainGame(const std::vector<dae::Scene*>& pScenes)
{
	// Create InGameUI scene
	m_pInGameUIScene = std::make_unique<InGameUIScene>(pScenes[1]);
}
void DigDugSceneManager::InitScenes(const std::vector<dae::Scene*>& pScenes)
{
	// Create main level
	m_pCurrentScene = std::make_unique<DigDugScene>(pScenes[2]);

	// Get level names
	m_LevelNames.emplace_back("Tiles/Level_1.tmj");
	m_LevelNames.emplace_back("Tiles/Level_2.tmj");
	m_LevelNames.emplace_back("Tiles/Level_3.tmj");
}

void DigDugSceneManager::GoToMenu()
{
	// Set changingLevel to true
	m_IsChangingLevel = true;

	// Deactive currentScene
	m_pCurrentScene->SetActive(false);
	m_pInGameUIScene->SetActive(false);

	// Go back to main menu, show score screen
	m_pUIScene->SetActive(true);
	m_pUIScene->ShowScoreScreen(true);

	// Reset currentLevel
	m_CurrentLevel = -1;

	// Set changingLevel to false
	m_IsChangingLevel = false;
}