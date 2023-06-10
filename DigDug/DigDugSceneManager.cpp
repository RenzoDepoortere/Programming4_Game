#include "DigDugSceneManager.h"

#include "Texture2D.h"
#include "RenderTextureComponent.h"
#include "GameObject.h"
#include "GridComponent.h"

#include "ServiceLocator.h"
#include "SDLSoundSystem.h"
#include "LoggingSoundSystem.h"
#include "ResourceManager.h"
#include "EventsEnum.h"
#include "EventManager.h"

#include "Scene.h"
#include <iostream>

using namespace digdug;

DigDugSceneManager::DigDugSceneManager()
	: m_CurrentLevel{ -1 }
{
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
		// Deactive currentScene
		m_pCurrentScene->SetActive(false);

		// Go back to main menu when over
		m_pUIScene->SetActive(true);

		// Reset currentLevel
		m_CurrentLevel = -1;

		return;
	}

	// Hide UI
	m_pUIScene->SetActive(false);

	// Set levelName
	m_pCurrentScene->SetLevel(m_LevelNames[m_CurrentLevel]);

	// Set changingLevel to false
	m_IsChangingLevel = false;
}

void DigDugSceneManager::HandleEvent(unsigned int eventID)
{
	if (eventID != event::PlayerDeath) return;

	// 1 live less
	// --> component calls whether to go to menu or reset

	m_pCurrentScene->Reset(false);
}
void DigDugSceneManager::OnSubjectDestroy()
{

}

void DigDugSceneManager::InitSystems()
{
	// Service Locator
	// ***************

	// Sound
#ifdef _DEBUG
	dae::ServiceLocator::RegisterSoundSystem(new dae::LoggingSoundSystem{ new dae::SDLSoundSystem{} });
#else
	dae::ServiceLocator::RegisterSoundSystem(new dae::SDLSoundSystem{});
#endif // DEBUG


}

void DigDugSceneManager::InitMenu(const std::vector<dae::Scene*>& pScenes)
{
	// Create UI scene
	m_pUIScene = std::make_unique<UIScene>(pScenes[1]);
}

void DigDugSceneManager::InitMainGame(const std::vector<dae::Scene*>& /*pScenes*/)
{
	// Subscribe to events
	dae::EventManager<>::GetInstance().Subscribe(event::PlayerDeath, this);

	// Score

	// Lives
}
void DigDugSceneManager::InitScenes(const std::vector<dae::Scene*>& pScenes)
{
	// Create main level
	m_pCurrentScene = std::make_unique<DigDugScene>(pScenes[2]);

	// Get level names
	m_LevelNames.emplace_back("Tiles/Level_1.tmj");
}