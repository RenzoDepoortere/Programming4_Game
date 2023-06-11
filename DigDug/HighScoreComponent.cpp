#include "HighScoreComponent.h"

#include "GameObject.h"
#include "TextComponent.h"
#include "Texture2D.h"

#include "ResourceManager.h"
#include "EventManager.h"
#include "EventsEnum.h"

#include <list>
#include <algorithm>
#include <string>

HighScoreComponent::HighScoreComponent(dae::GameObject* pParentObject)
	: Component{ pParentObject }
{
	// Create textComponent
	// --------------------
	auto font = dae::ResourceManager::GetInstance().LoadFont("Other/Arcade.ttf", 48);
	auto pTexture = std::make_shared<dae::Texture2D>(nullptr);

	m_pTextComponent = GetGameObject()->AddComponent<dae::TextComponent>();
	m_pTextComponent->SetText("0000");
	m_pTextComponent->SetFont(font);
	m_pTextComponent->SetTexture(pTexture);

	// Get highestScore
	// ----------------
	
	// Get file
	auto pHighscoreFile{ dae::ResourceManager::GetInstance().LoadReadFile("Other/HighScore.txt") };
	if (pHighscoreFile->is_open())
	{
		// Go through all the lines
		std::list<int> scores{};
		std::string readLine{};
		std::string number{};
		while (std::getline(*pHighscoreFile, readLine))
		{
			// Get the number, is after the '/'
			number = readLine.substr(readLine.find('/') + 1);
			scores.emplace_back(std::stoi(number));
		}

		// Sort the list
		scores.sort(std::greater<int>());

		// Get and set the highestScore
		if (scores.size() != 0)
		{
			m_HighestScore = scores.front();
			m_pTextComponent->SetText(std::to_string(m_HighestScore));
		}
	}
	else
	{
		std::cout << "Error: failed openining the highscore file" << std::endl;
	}

	// Subscribe to events
	// -------------------
	dae::EventManager<int>::GetInstance().Subscribe(event::NewHighScore, this);
}
HighScoreComponent::~HighScoreComponent()
{
	// Unsubscribe from events
	if (dae::EventManager<int>::GetIsDestroyed() == false)
	{
		dae::EventManager<int>::GetInstance().Unsubscribe(event::NewHighScore, this);
	}
}

void HighScoreComponent::HandleEvent(unsigned int /*eventID*/, int newHighScore)
{
	// Set text
	m_pTextComponent->SetText(std::to_string(newHighScore));
}
void HighScoreComponent::OnSubjectDestroy()
{
}