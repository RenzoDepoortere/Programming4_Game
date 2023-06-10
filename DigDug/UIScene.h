#pragma once

namespace dae
{
	class GameObject;
	class Scene;
}

class UIScene final
{
public:
	// Rule of five
	UIScene(dae::Scene* pScene);
	~UIScene() = default;

	UIScene(const UIScene& other) = delete;
	UIScene(UIScene&& other) = delete;
	UIScene& operator=(const UIScene& other) = delete;
	UIScene& operator=(UIScene&& other) = delete;

	// Functionality
	void SetActive(bool isActive);

private:
	// Member variables
	// ----------------
	dae::GameObject* m_pSceneRootObject{ nullptr };

	// Member functions
	// ----------------
	void InitWidgets();
};

