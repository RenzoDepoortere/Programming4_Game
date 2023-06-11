#pragma once
#include <vector>

namespace dae
{
	class GameObject;
	class Scene;
}

class SelectionComponent;
class ScoreInputComponent;

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
	void ShowScoreScreen(bool showScreen);

private:
	// Member variables
	// ----------------
	dae::GameObject* m_pSceneRootObject{ nullptr };
	std::vector<float> m_ButtonYPos{};

	dae::GameObject* m_pMenuObject{ nullptr };
	ScoreInputComponent* m_pScoreInputComponent{ nullptr };

	// Member functions
	// ----------------
	void InitWidgets();
	void InitArrow();
	void InitScoreScreen();
};

