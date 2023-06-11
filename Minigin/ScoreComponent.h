#pragma once
#include "Component.h"
#include "Observer.h"
#include "../DigDug/GridComponent.h"

#include <list>

namespace dae
{
	class GameObject;
	class TextComponent;
}

class ScoreComponent final : public Component, dae::Observer<grid::Cell*, void*, void*>
{
public:
	// Rule of Five
	explicit ScoreComponent(dae::GameObject* pParentObject);
	virtual ~ScoreComponent() override;

	ScoreComponent(const ScoreComponent& other) = delete;
	ScoreComponent(ScoreComponent&& other) = delete;
	ScoreComponent& operator=(const ScoreComponent& other) = delete;
	ScoreComponent& operator=(ScoreComponent&& other) = delete;

	// Functionality
	void SetScores(const std::list < std::string> & scores) { m_Scores = scores; }
	void SetHighestScore(int highestScore) { m_HighestScore = highestScore; }

	void StoreScore();

	// Observer
	virtual void HandleEvent(unsigned int eventID, grid::Cell* pCell, void* extraInfo_1, void* extraInfo_2) override;
	virtual void OnSubjectDestroy() override;

private:
	// Member variables
	// ----------------
	dae::TextComponent* m_pTextComponent{ nullptr };
	int m_CurrentScore{};

	std::list<std::string> m_Scores{};
	int m_HighestScore{};

	// Member functions
	// ----------------
	int HandleEnemyDeath(grid::Cell* pCell, void* extraInfo_1);
	int HandleRockBreak(void* extraInfo_1);
};