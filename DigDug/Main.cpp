#include <SDL.h>
#include <iostream>
#include <vector>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "Scene.h"
#include "DigDugSceneManager.h"

void load()
{
	std::vector<dae::Scene*> pScenes{};
	pScenes.emplace_back(dae::SceneManager::GetInstance().CreateScene("UIScene"));
	pScenes.emplace_back(dae::SceneManager::GetInstance().CreateScene("MainGameScene"));
	pScenes.emplace_back(dae::SceneManager::GetInstance().CreateScene("LevelScene"));

	digdug::DigDugSceneManager::GetInstance().Initialize(pScenes);
}

int main(int, char* [])
{
	// Engine
	std::string dataPath{"Data/"};
#ifdef _DEBUG
	//dataPath = "../Data/";
#endif // DEBUG

	dae::Minigin engine(dataPath);
	engine.Run(load);

	// Close
	return 0;
}