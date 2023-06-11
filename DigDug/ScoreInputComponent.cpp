#include "ScoreInputComponent.h"

#include "GameObject.h"
#include "TextComponent.h"
#include "Texture2D.h"

#include "DigDugSceneManager.h"

#include "ResourceManager.h"
#include "EventManager.h"
#include "EventsEnum.h"

#include <iostream>
#include <string>
#include <memory>

ScoreInputComponent::ScoreInputComponent(dae::GameObject* pParentObject)
	: Component{ pParentObject }
	, m_MaxCooldown{ 0.25f }
	, m_MaxScores{ 5 }
{
	// Create X textComponents
	// -----------------------

	std::shared_ptr<dae::GameObject> pGameObject{ nullptr };
	auto font = dae::ResourceManager::GetInstance().LoadFont("Other/Arcade.ttf", 64);
	std::shared_ptr<dae::Texture2D> pTexture{ nullptr };
	dae::TextComponent* pTextComponent{ nullptr };

	auto pOwnGameObject{ GetGameObject() };
	const float startPos{ 20 };
	const float distanceBetween{ 60 };

	for (int idx{}; idx < m_MaxScores; ++idx)
	{
		// Create gameObject
		pGameObject = std::make_shared<dae::GameObject>();

		// Add components
		pTexture = std::make_shared<dae::Texture2D>(nullptr);

		pTextComponent = pGameObject->AddComponent<dae::TextComponent>();
		pTextComponent->SetText("TST 1234");
		pTextComponent->SetFont(font);
		pTextComponent->SetTexture(pTexture);

		// Add as child
		pGameObject->SetParent(pOwnGameObject, false);
		m_pRenderTextures.emplace_back(pTextComponent);

		// Set Position
		pGameObject->SetWorldPosition(0, startPos + idx * distanceBetween, 0);
	}


	// Subscribe to events
	// -------------------
	dae::EventManager<float>::GetInstance().Subscribe(event::StartMenu, this);

	dae::EventManager<float>::GetInstance().Subscribe(event::KeyboardLeft, this);
	dae::EventManager<float>::GetInstance().Subscribe(event::KeyboardRight, this);
	dae::EventManager<float>::GetInstance().Subscribe(event::KeyboardUp, this);
	dae::EventManager<float>::GetInstance().Subscribe(event::KeyboardDown, this);

	dae::EventManager<float>::GetInstance().Subscribe(event::ControllerLeft_2, this);
	dae::EventManager<float>::GetInstance().Subscribe(event::ControllerRight_2, this);	
	dae::EventManager<float>::GetInstance().Subscribe(event::ControllerUp_2, this);
	dae::EventManager<float>::GetInstance().Subscribe(event::ControllerDown_2, this);

	dae::EventManager<float>::GetInstance().Subscribe(event::ControllerLeft_1, this);
	dae::EventManager<float>::GetInstance().Subscribe(event::ControllerRight_1, this);
	dae::EventManager<float>::GetInstance().Subscribe(event::ControllerUp_1, this);
	dae::EventManager<float>::GetInstance().Subscribe(event::ControllerDown_1, this);
}
ScoreInputComponent::~ScoreInputComponent()
{
	// Unsubscribe to events
	if (dae::EventManager<float>::GetIsDestroyed() == false)
	{
		dae::EventManager<float>::GetInstance().Unsubscribe(event::StartMenu, this);

		dae::EventManager<float>::GetInstance().Unsubscribe(event::KeyboardLeft, this);
		dae::EventManager<float>::GetInstance().Unsubscribe(event::KeyboardRight, this);
		dae::EventManager<float>::GetInstance().Unsubscribe(event::KeyboardUp, this);
		dae::EventManager<float>::GetInstance().Unsubscribe(event::KeyboardDown, this);

		dae::EventManager<float>::GetInstance().Unsubscribe(event::ControllerLeft_2, this);
		dae::EventManager<float>::GetInstance().Unsubscribe(event::ControllerRight_2, this);
		dae::EventManager<float>::GetInstance().Unsubscribe(event::ControllerUp_2, this);
		dae::EventManager<float>::GetInstance().Unsubscribe(event::ControllerDown_2, this);

		dae::EventManager<float>::GetInstance().Unsubscribe(event::ControllerLeft_1, this);
		dae::EventManager<float>::GetInstance().Unsubscribe(event::ControllerRight_1, this);
		dae::EventManager<float>::GetInstance().Unsubscribe(event::ControllerUp_1, this);
		dae::EventManager<float>::GetInstance().Unsubscribe(event::ControllerDown_1, this);
	}
}

void ScoreInputComponent::Update(float deltaTime)
{
	if (0.f < m_Cooldown)
	{
		m_Cooldown -= deltaTime;
	}
}

void ScoreInputComponent::HandleEvent(unsigned int eventID, float /*deltaTime*/)
{
	// Check if is active
	if (GetGameObject()->GetIsActive() == false) return;

	// Check eventID
	switch (eventID)
	{
	case event::StartMenu:
		StoreScore();
		break;

	case event::KeyboardLeft:
	case event::ControllerLeft_1:
	case event::ControllerLeft_2:
		GoLeft();
		break;

	case event::KeyboardRight:
	case event::ControllerRight_1:
	case event::ControllerRight_2:
		GoRight();
		break;

	case event::KeyboardUp:
	case event::ControllerUp_1:
	case event::ControllerUp_2:
		GoUp();
		break;

	case event::KeyboardDown:
	case event::ControllerDown_1:
	case event::ControllerDown_2:
		GoDown();
		break;
	}
}
void ScoreInputComponent::OnSubjectDestroy()
{
}

void ScoreInputComponent::StoreScore()
{
	// Get 3 letters
	// -------------


	// Set score to file
	// -----------------
	auto& sceneManager = digdug::DigDugSceneManager::GetInstance();

	// Get file
	auto pHighscoreFile{ dae::ResourceManager::GetInstance().LoadWriteFile("Other/HighScore.txt") };
	if (pHighscoreFile->is_open())
	{
		// Get data
		const int currentScore{ sceneManager.GetCurrentScore() };
		auto scores{ sceneManager.GetScores() };

		// Remove last if over max
		if (m_MaxScores <= scores.size()) scores.pop_back();

		// Add own score
		const std::string scoreString{ "/" + std::to_string(currentScore) };
		scores.emplace_back(scoreString);

		// Sort
		SortList(scores);

		// Write to file
		for (const auto& score : scores)
		{
			*pHighscoreFile << score << '\n';
		}
	}
	else
	{
		std::cout << "Error: failed openining the highscore file" << std::endl;
	}

	// Reset score and continue
	// ------------------------
	sceneManager.ResetScore();
	sceneManager.ShowScoreScreen(false);
}
void ScoreInputComponent::SortList(std::list<std::string>& scores)
{
	// Sort list based on score
	auto descendingLambda = [](const std::string& a, const std::string& b)
	{
		const int firstNumber = std::stoi(a.substr(a.find('/') + 1));
		const int secondNumber = std::stoi(b.substr(b.find('/') + 1));

		return firstNumber > secondNumber;
	};
	scores.sort(descendingLambda);
}

void ScoreInputComponent::GoLeft()
{

}
void ScoreInputComponent::GoRight()
{

}
void ScoreInputComponent::GoDown()
{

}
void ScoreInputComponent::GoUp()
{

}