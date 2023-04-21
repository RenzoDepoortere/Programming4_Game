#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update(float deltaTime)
{
	// Update scenes
	for (auto& scene : m_scenes)
	{
		scene->Update(deltaTime);
	}
}

void dae::SceneManager::FixedUpdate(float deltaTime)
{
	// FixedUpdate scenes
	for (const auto& scene : m_scenes)
	{
		scene->FixedUpdate(deltaTime);
	}
}

void dae::SceneManager::Render() const
{
	// Render scenes
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
}

void dae::SceneManager::RenderImGUI()
{
	// Render ImGUI scenes
	for (const auto& scene : m_scenes)
	{
		scene->RenderImGUI();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}
