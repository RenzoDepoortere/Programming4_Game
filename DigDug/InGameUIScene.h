#pragma once
#include "LivesComponent.h"
#include "ScoreComponent.h"
#include "HighScoreComponent.h"

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
	void ResetScore() { m_pScoreComponent->ResetScore(); }

	int GetCurrentScore() const { return m_pScoreComponent->GetScore(); }
	const std::list<std::string>& GetScores() const { return m_pHighscoreComponent->GetScores(); }

private:
	// Member variables
	// ----------------
	dae::GameObject* m_pSceneRootObject{ nullptr };
	LivesComponent* m_pLivesComponent{ nullptr };
	ScoreComponent* m_pScoreComponent{ nullptr };
	HighScoreComponent* m_pHighscoreComponent{ nullptr };

	// Member functions
	// ----------------
	void InitScore();
	void InitLives();
};