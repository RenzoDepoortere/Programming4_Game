#pragma once
#include "LivesComponent.h"

namespace dae
{
	class GameObject;
	class Scene;
}

class InGameUIScene final
{
public:
	// Rule of five
	InGameUIScene(dae::Scene* pScene);
	~InGameUIScene() = default;

	InGameUIScene(const InGameUIScene& other) = delete;
	InGameUIScene(InGameUIScene&& other) = delete;
	InGameUIScene& operator=(const InGameUIScene& other) = delete;
	InGameUIScene& operator=(InGameUIScene&& other) = delete;

	// Functionality
	void SetActive(bool isActive);
	bool LoseLive() { return m_pLivesComponent->LoseLive(); }

private:
	// Member variables
	// ----------------
	dae::GameObject* m_pSceneRootObject{ nullptr };
	LivesComponent* m_pLivesComponent{ nullptr };


	// Member functions
	// ----------------
	void InitScore();
	void InitLives();
};