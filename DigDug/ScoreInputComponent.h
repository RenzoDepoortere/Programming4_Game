#pragma once
#include "Component.h"
#include "Observer.h"

#include <list>
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


	// Observer
	virtual void HandleEvent(unsigned int eventID, float deltaTime) override;
	virtual void OnSubjectDestroy() override;

private:
	// Member variables
	// ----------------
	dae::TextComponent* m_pTextComponent{ nullptr };

	float m_Cooldown{};
	float m_MaxCooldown{};

	// Member functions
	// ----------------
	void StoreScore();
	void SortList(std::list<std::string>& scores);

	void GoLeft();
	void GoRight();
	void GoDown();
	void GoUp();
};
