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
	, m_MaxLetters{ 3 }
{
	m_Letters.resize(m_MaxLetters);

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
	// Cooldown
	if (0.f < m_InputCooldown)
	{
		m_InputCooldown -= deltaTime;
	}

	// Flicker current letter
	HandleFlickering(deltaTime);
}
void ScoreInputComponent::InputStart()
{
	// Reset variables
	// ---------------
	m_CurrentLetterIdx = 0;
	m_InputCooldown = 0.f;

	m_pTextComponentToChange = nullptr;
	m_CurrentFlickerTimer = 0.f;
	m_IsShowingLetter = true;

	std::fill(m_Letters.begin(), m_Letters.end(), 'A');

	// Get data
	// --------
	auto& sceneManager = digdug::DigDugSceneManager::GetInstance();

	// Get scores
	const int currentScore{ sceneManager.GetCurrentScore() };
	auto scores{ sceneManager.GetScores() };

	// Remove last if over max
	if (m_MaxScores <= scores.size()) scores.pop_back();

	// Add own score
	const std::string scoreString{ "AAA/" + std::to_string(currentScore) };
	scores.emplace_back(scoreString);

	// Sort
	SortList(scores);

	// Set text of textComponents
	// --------------------------
	dae::TextComponent* pTextComponent{ nullptr };
	int idx{};

	// Loop through scores
	for (const auto& score : scores)
	{
		// Get textComponent
		pTextComponent = m_pRenderTextures[idx];
		++idx;

		// Set text
		SetText(pTextComponent, score);

		// Set color
		pTextComponent->SetColor(SDL_Color{ 255, 255, 255, 0 });

		// Check if is currentScore
		if (score == scoreString)
		{
			// Set different color and store
			pTextComponent->SetColor(SDL_Color{ 220, 0, 0, 0 });
			m_pTextComponentToChange = pTextComponent;

			continue;
		}
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
	const std::string letters{ m_Letters.begin(), m_Letters.end() };

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
		const std::string scoreString{ letters + "/" + std::to_string(currentScore) };
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
void ScoreInputComponent::SetText(dae::TextComponent* pTextComponent, const std::string& string)
{
	std::string text{};
	size_t offset{ string.find('/') };

	text = string.substr(0, offset);
	text += " ";
	text += string.substr(offset + 1);

	pTextComponent->SetText(text);
}

void ScoreInputComponent::HandleFlickering(float deltaTime)
{
	if (m_pTextComponentToChange == nullptr) return;

	// Cooldown
	m_CurrentFlickerTimer += deltaTime;
	const float threshold{ 0.3f };
	if (threshold < m_CurrentFlickerTimer)
	{
		m_CurrentFlickerTimer = 0.f;

		// Change current letter
		std::string currentString{ m_pTextComponentToChange->GetText() };
		if (m_IsShowingLetter)
		{
			currentString[m_CurrentLetterIdx] = ' ';
		}
		else
		{
			currentString[m_CurrentLetterIdx] = m_Letters[m_CurrentLetterIdx];
		}

		// Set variables
		m_pTextComponentToChange->SetText(currentString);
		m_IsShowingLetter = !m_IsShowingLetter;
	}
}

void ScoreInputComponent::GoLeft()
{
	// Check if can go left
	if (m_CurrentLetterIdx == 0) return;

	// Return if on cooldown
	if (0.f < m_InputCooldown) return;
	m_InputCooldown = m_MaxCooldown;

	// Reset text
	std::string currentString{ m_pTextComponentToChange->GetText() };
	currentString[m_CurrentLetterIdx] = m_Letters[m_CurrentLetterIdx];
	m_pTextComponentToChange->SetText(currentString);

	// Lower one
	--m_CurrentLetterIdx;
}
void ScoreInputComponent::GoRight()
{
	// Check if can go right
	if (m_CurrentLetterIdx == m_MaxLetters - 1) return;

	// Return if on cooldown
	if (0.f < m_InputCooldown) return;
	m_InputCooldown = m_MaxCooldown;

	// Reset text
	std::string currentString{ m_pTextComponentToChange->GetText() };
	currentString[m_CurrentLetterIdx] = m_Letters[m_CurrentLetterIdx];
	m_pTextComponentToChange->SetText(currentString);

	// Go up one
	++m_CurrentLetterIdx;
}
void ScoreInputComponent::GoDown()
{
	// Check if can go down one
	if (m_Letters[m_CurrentLetterIdx] == 'A') return;

	// Return if on cooldown
	if (0.f < m_InputCooldown) return;
	m_InputCooldown = m_MaxCooldown;

	// Lower one
	--m_Letters[m_CurrentLetterIdx];

	// Set text
	std::string currentString{ m_pTextComponentToChange->GetText() };
	currentString[m_CurrentLetterIdx] = m_Letters[m_CurrentLetterIdx];
	m_pTextComponentToChange->SetText(currentString);
}
void ScoreInputComponent::GoUp()
{
	// Check if can go up one
	if (m_Letters[m_CurrentLetterIdx] == 'Z') return;

	// Return if on cooldown
	if (0.f < m_InputCooldown) return;
	m_InputCooldown = m_MaxCooldown;

	// Go up one
	++m_Letters[m_CurrentLetterIdx];

	// Set text
	std::string currentString{ m_pTextComponentToChange->GetText() };
	currentString[m_CurrentLetterIdx] = m_Letters[m_CurrentLetterIdx];
	m_pTextComponentToChange->SetText(currentString);
}