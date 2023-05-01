#include <SDL.h>
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
#include "FirstScene.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Level_1");
	FirstScene::GetInstance().CreateGameObjects(scene);
}

int main(int, char* [])
{
	// Engine
	dae::Minigin engine("../Data/");
	engine.Run(load);

	// Close
	return 0;
}