#include <SDL.h>
#include <steam_api.h>
#include <iostream>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObjectManager.h"
#include "AchievementWrapper.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	dae::GameObjectManager::GetInstance().CreateGameObjects(scene);
}

int main(int, char*[])
{
	// Steam
	if (!SteamAPI_Init())
	{
		std::cerr << "Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)." << std::endl;
		return 1;
	}
	else
	{
		std::cout << "Successfully initialized steam." << std::endl;
	}
	dae::AchievementWrapper::GetInstance().Initialize();

	// Engine
	dae::Minigin engine("../Data/");
	engine.Run(load);

	// Close
	SteamAPI_Shutdown();
    return 0;
}