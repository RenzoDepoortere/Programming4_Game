#pragma once
#include "Component.h"
#include "Observer.h"

#include <fstream>
#include <memory>

namespace dae
{
	class GameObject;
	class TextComponent;
}

class HighScoreComponent final : public Component, public dae::Observer<int>
{
public:
	// Rule of Five
	explicit HighScoreComponent(dae::GameObject* pParentObject);
	virtual ~HighScoreComponent() override;

	HighScoreComponent(const HighScoreComponent& other) = delete;
	HighScoreComponent(HighScoreComponent&& other) = delete;
	HighScoreComponent& operator=(const HighScoreComponent& other) = delete;
	HighScoreComponent& operator=(HighScoreComponent&& other) = delete;

	// Functionality
	int GetHighestScore() const { return m_HighestScore; }

	// Observer
	virtual void HandleEvent(unsigned int eventID, int newHighScore) override;
	virtual void OnSubjectDestroy() override;

private:
	// Member variables
	// ----------------
	dae::TextComponent* m_pTextComponent{ nullptr };
	int m_HighestScore{};
};