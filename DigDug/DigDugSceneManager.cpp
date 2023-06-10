#include "DigDugSceneManager.h"

#include "Texture2D.h"
#include "RenderTextureComponent.h"
#include "GameObject.h"
#include "GridComponent.h"

#include "ServiceLocator.h"
#include "SDLSoundSystem.h"
#include "LoggingSoundSystem.h"
#include "ResourceManager.h"

#include "Scene.h"
#include "FirstScene.h"

#include <iostream>

using namespace digdug;

DigDugSceneManager::~DigDugSceneManager()
{
	dae::ServiceLocator::Shutdown();
}

void DigDugSceneManager::Initialize(const std::vector<dae::Scene*>& pScenes)
{
	InitSystems();
	InitMainGame(pScenes);
	InitScenes(pScenes);
}
void DigDugSceneManager::SetLevel(int levelIndex)
{
	// If idx above max, return
	if (m_pScenes.size() <= levelIndex)
	{
		std::cout << "Error: tried to set levelIndex above max" << std::endl;
		return;
	}

	// Stop current scene and set next scene
	m_pCurrentScene->SetActive(false);
	m_pCurrentScene = m_pScenes[levelIndex].get();
	m_pCurrentScene->SetActive(true);
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
void DigDugSceneManager::InitMainGame(const std::vector<dae::Scene*>& /*pScenes*/)
{
	
}
void DigDugSceneManager::InitScenes(const std::vector<dae::Scene*>& pScenes)
{
	// Scene 1
	m_pScenes[0] = std::make_unique<FirstScene>(pScenes[1]);

	// Set main level
	m_pCurrentScene = m_pScenes[0].get();
	m_pCurrentScene->SetActive(true);
}