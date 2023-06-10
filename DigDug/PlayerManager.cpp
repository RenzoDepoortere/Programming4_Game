#include "PlayerManager.h"

#include "AnimationComponent.h"
#include "GameObject.h"
#include "CharacterComponent.h"
#include "GridComponent.h"

#include "DigDugSceneManager.h"

#include "EventManager.h"
#include "EventsEnum.h"

#include <algorithm>

PlayerManager::PlayerManager(dae::GameObject* pParentObject)
	: Component{ pParentObject }
{
}

void PlayerManager::Reset()
{
	// Reset players
	// -------------

	// Get center cell
	grid::GridComponent* pGrid{ digdug::DigDugSceneManager::GetInstance().GetGrid() };
	grid::Cell* pCell{ pGrid->GetCell(0) };

	// Loop through
	for (int idx{}; idx < m_pCharacters.size(); ++idx)
	{
		// Reset player
		m_pCharacters[idx]->Reset();

		// Set position
		m_pCharacters[idx]->GetGameObject()->SetWorldPosition(pCell->centerPosition);
	}
}
void PlayerManager::PlayerDeath()
{
	// Check whether players left
	auto isAlive = [](CharacterComponent* pCharacter)
	{
		return pCharacter->GetGameObject()->GetIsActive();
	};
	const int nrAlivePlayers{ static_cast<int>(std::count_if(m_pCharacters.begin(), m_pCharacters.end(), isAlive)) };

	// If no more alive players
	if (nrAlivePlayers == 0)
	{
		// Send death event
		dae::EventManager<>::GetInstance().SendEvent(event::PlayerDeath);
	}
}

void PlayerManager::SpawnPlayers()
{
	for (int idx{}; idx < m_NrPlayers; ++idx)
	{
		Spawn(idx);
	}
}

void PlayerManager::Spawn(int index)
{
	// Create gameObject
	// -----------------
	std::shared_ptr<dae::GameObject> pMainCharacter{ std::make_shared<dae::GameObject>() };

	// Add texture
	// -----------	
	dae::AnimationComponent* pObjectTexture{ pMainCharacter->AddComponent<dae::AnimationComponent>() };
	pObjectTexture->CenterTexture(true);

	pObjectTexture->SetSingleSpriteSize(25.f);
	pObjectTexture->SetMaxFrames(2);
	pObjectTexture->SetFramesPerSecond(12);

	pObjectTexture->SetPaused(true);

	// Add characterComponent
	// ----------------------
	CharacterComponent* pCharacterComponent{ pMainCharacter->AddComponent<CharacterComponent>() };
	pCharacterComponent->StoreParent(GetGameObject());

	// Add to root
	// -----------
	m_pCharacters.emplace_back(pCharacterComponent);
	pMainCharacter->SetParent(GetGameObject(), true);

	// Transform
	// ---------
	grid::GridComponent* pGrid{ digdug::DigDugSceneManager::GetInstance().GetGrid() };

	// First left
	grid::Cell* pCell{ pGrid->GetCell(index) };
	pMainCharacter->SetWorldPosition(pCell->centerPosition);

	// Second right
}