#include "ScoreComponent.h"

#include "GameObject.h"
#include "TextComponent.h"
#include "Texture2D.h"

#include "EventManager.h"
#include "../DigDug/EventsEnum.h"
#include "ResourceManager.h"
#include "../DigDug/EnemyComponent.h"
#include "../DigDug/CharacterComponent.h"

#include "../DigDug/DigDugSceneManager.h"

#include <memory>

ScoreComponent::ScoreComponent(dae::GameObject* pParentObject)
	: Component{ pParentObject }
{
	// Create textComponent
	auto font = dae::ResourceManager::GetInstance().LoadFont("Other/Arcade.ttf", 48);
	auto pTexture = std::make_shared<dae::Texture2D>(nullptr);

	m_pTextComponent = GetGameObject()->AddComponent<dae::TextComponent>();
	m_pTextComponent->SetText("0000");
	m_pTextComponent->SetFont(font);
	m_pTextComponent->SetTexture(pTexture);

	// Subscribe to events
	auto& eventManager{ dae::EventManager<grid::Cell*, void*, void*>::GetInstance() };
	eventManager.Subscribe(event::EnemyDeath, this);
}
ScoreComponent::~ScoreComponent()
{
	// Unsubscribe from events
	if (dae::EventManager<>::GetIsDestroyed() == false)
	{
		auto& eventManager{ dae::EventManager<grid::Cell*, void*, void*>::GetInstance() };
		eventManager.Unsubscribe(event::EnemyDeath, this);
	}
}

void ScoreComponent::StoreScore()
{
	// Ask for 3 letters

	// Set score to file

	// Reset score
	m_CurrentScore = 0;
	m_pTextComponent->SetText(std::to_string(m_CurrentScore));
}

void ScoreComponent::HandleEvent(unsigned int eventID, grid::Cell* pCell, void* extraInfo_1, void* /*extraInfo_2*/)
{
	int scoreGain{};

	// Calculate score
	switch (eventID)
	{
	case event::EnemyDeath:
		scoreGain = HandleEnemyDeath(pCell, extraInfo_1);
		break;

	case event::RockBreak:
		scoreGain = HandleRockBreak(extraInfo_1);
		break;
	}

	m_CurrentScore += scoreGain;

	// Set text
	m_pTextComponent->SetText(std::to_string(m_CurrentScore));
}
void ScoreComponent::OnSubjectDestroy()
{
}

int ScoreComponent::HandleEnemyDeath(grid::Cell* pCell, void* extraInfo_1)
{
	// Get data
	// --------
	enemy::BehaviorData* pBehaviorData{ static_cast<enemy::BehaviorData*>(extraInfo_1) };
	const bool isFygar{ pBehaviorData->enemyType == enemy::Fygar };

	// Check depthLevel
	// ---------------
	int scoreGain{};
	int depthLevel{ 3 };
	if (pCell) depthLevel = pCell->depthLevel;
	
	switch (depthLevel)
	{
	case 0:
		scoreGain = 200;
		break;

	case 1:
		scoreGain = 300;
		break;

	case 2:
		scoreGain = 400;
		break;

	case 3:
		scoreGain = 500;
		break;
	}
	

	// Check whether player was in same row
	// ------------------------------------

	// If fygar
	if (isFygar)
	{
		grid::GridComponent* pGrid{ digdug::DigDugSceneManager::GetInstance().GetGrid() };
		grid::Cell* pCurrentCell{ nullptr };

		int row{ pGrid->GetNrRows() - 1 };
		if (pCell) row = static_cast<int>(pCell->rowCol.x);

		// Loop through player
		for (const auto& currentPlayer : digdug::DigDugSceneManager::GetInstance().GetCharacters())
		{
			// If in same row
			pCurrentCell = pGrid->GetCell(currentPlayer->GetGameObject()->GetWorldPosition());
			if (pCurrentCell == nullptr) continue;

			if (pCurrentCell->rowCol.x == row)
			{
				// Multiply score
				scoreGain *= 2;
				break;
			}
		}
	}

	// Return
	// ------
	return scoreGain;
}
int ScoreComponent::HandleRockBreak(void* extraInfo_1)
{
	// Get data
	dae::GameObject* pGameObject{ static_cast<dae::GameObject*>(extraInfo_1) };
	int nrCaughtEnemies{ static_cast<int>(pGameObject->GetChildren().size()) };

	// Check if player was caught too
	for (const auto& currentPlayer : digdug::DigDugSceneManager::GetInstance().GetCharacters())
	{
		if (currentPlayer->GetGameObject()->GetParent() == pGameObject)
		{
			--nrCaughtEnemies;
		}
	}

	// Calculate score
	int scoreGain{};
	switch (nrCaughtEnemies)
	{
	case 1:
		scoreGain = 1'000;
		break;

	case 2:
		scoreGain = 2'500;
		break;

	case 3:
		scoreGain = 4'000;
		break;

	case 4:
		scoreGain = 6'000;
		break;

	case 5:
		scoreGain = 8'000;
		break;

	case 6:
		scoreGain = 10'000;
		break;

	case 7:
		scoreGain = 12'000;
		break;

	case 8:
		scoreGain = 15'000;
		break;
	}

	// return
	return scoreGain;
}