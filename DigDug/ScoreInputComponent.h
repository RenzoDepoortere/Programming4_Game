#pragma once
#include "Component.h"
#include "Observer.h"

#include <list>
#include <vector>
#include <string>

namespace dae
{
	class GameObject;
	class TextComponent;
}

class ScoreInputComponent final : public Component, dae::Observer<float>
{
public:
	// Rule of Five
	explicit ScoreInputComponent(dae::GameObject* pParentObject);
	virtual ~ScoreInputComponent() override;

	ScoreInputComponent(const ScoreInputComponent& other) = delete;
	ScoreInputComponent(ScoreInputComponent&& other) = delete;
	ScoreInputComponent& operator=(const ScoreInputComponent& other) = delete;
	ScoreInputComponent& operator=(ScoreInputComponent&& other) = delete;

	// Functionality
	virtual void Update(float deltaTime) override;
	void InputStart();

	// Observer
	virtual void HandleEvent(unsigned int eventID, float deltaTime) override;
	virtual void OnSubjectDestroy() override;

private:
	// Member variables
	// ----------------
	dae::TextComponent* m_pTextComponent{ nullptr };

	float m_InputCooldown{};
	float m_MaxCooldown{};

	int m_MaxScores{};
	std::vector<dae::TextComponent*> m_pRenderTextures{};
	dae::TextComponent* m_pTextComponentToChange{};

	int m_CurrentLetterIdx{};
	int m_MaxLetters{};
	std::vector<char> m_Letters{};

	float m_CurrentFlickerTimer{};
	bool m_IsShowingLetter{};

	// Member functions
	// ----------------
	void StoreScore();
	void SortList(std::list<std::string>& scores);
	void SetText(dae::TextComponent* pTextComponent, const std::string& string);

	void HandleFlickering(float deltaTime);

	void GoLeft();
	void GoRight();
	void GoDown();
	void GoUp();
};
