#include "EnemyManager.h"

#include "GameObject.h"
#include "GridComponent.h"
#include "AnimationComponent.h"	
#include "EnemyComponent.h"
#include "CharacterComponent.h"

#include "DigDugSceneManager.h"

#include "ResourceManager.h"
#include "EventManager.h"
#include "EventsEnum.h"
#include "ServiceLocator.h"

#include <algorithm>

EnemyManager::EnemyManager(dae::GameObject* pParentObject)
	: Component{ pParentObject }
{
	// Subscribe to enemyDeath event
	dae::EventManager<grid::Cell*, void*, void*>::GetInstance().Subscribe(event::EnemyDeath, this);

	// Create SFX
	const std::string fileName{ "Sound/Characters/Enemies/FinalEnemy.wav" };
	dae::ServiceLocator::GetSoundSystem().SetID(event::FinalEnemy, fileName);
}
EnemyManager::~EnemyManager()
{
	// Unsubscribe to enemyDeath event
	if (dae::EventManager<>::GetIsDestroyed() == false)
	{
		dae::EventManager<grid::Cell*, void*, void*>::GetInstance().Unsubscribe(event::EnemyDeath, this);
	}
}

void EnemyManager::Reset()
{
	// Reset enemies
	// -------------
	auto spawnData{ digdug::DigDugSceneManager::GetInstance().GetGrid()->GetEnemySpawnData() };

	// Loop through
	for (size_t idx{}; idx < m_pEnemies.size(); ++idx)
	{
		// Reset enemy
		m_pEnemies[idx]->Reset();

		// Set position
		m_pEnemies[idx]->GetGameObject()->SetWorldPosition(spawnData[idx].first);
	}
}

void EnemyManager::SpawnEnemies()
{
	// Loop through data
	for (const auto& currentData : digdug::DigDugSceneManager::GetInstance().GetGrid()->GetEnemySpawnData())
	{
		// If ID is 0, spawn Pooka
		if (currentData.second == 0) SpawnPooka(currentData.first);
		// Else, spawn Fygar
		else						 SpawnFygar(currentData.first);
	}
}

void EnemyManager::ControlEnemy(enemy::EnemyTypes enemyType)
{
	// Loop through enemies
	for (const auto& currentEnemy : m_pEnemies)
	{
		// Check if enemy is desired type
		if (currentEnemy->GetBehaviorData().enemyType == enemyType)
		{
			// Set control of enemy to controller
			currentEnemy->SetIsControlled(true);
			return;
		}
	}

	// State that enemyType was not found
	std::cout << "Error: Requested enemyType not found" << std::endl;
}

void EnemyManager::HandleEvent(unsigned int /*eventID*/, grid::Cell* /*pCell*/, void* /*behaviorData*/, void* /*wasSquashed*/)
{
	auto isAlive = [](EnemyComponent* pEnemy)
	{
		return pEnemy->GetGameObject()->GetIsActive();
	};

	// Check if there's just 1 enemy alive
	const int nrAliveEnemies = static_cast<int>(std::count_if(m_pEnemies.begin(), m_pEnemies.end(), isAlive));
	if (nrAliveEnemies == 1)
	{
		// Get alive enemy
		auto enemyIt = std::find_if(m_pEnemies.begin(), m_pEnemies.end(), isAlive);
		EnemyComponent* pEnemy{ *enemyIt };

		// Set enemy to flee state
		pEnemy->SetFlee();

		// Play SFX
		const int volume{ 100 };
		const int loops{ 0 };
		dae::ServiceLocator::GetSoundSystem().PlayAudio(event::FinalEnemy, volume, loops, 1);
	}

	// If there's no enemies left
	if (nrAliveEnemies == 0)
	{
		// Go to next level
		digdug::DigDugSceneManager::GetInstance().NextLevel();
	}
}
void EnemyManager::OnSubjectDestroy()
{
}

void EnemyManager::SpawnPooka(const glm::vec3& position)
{
	// Create gameObject
	// -----------------
	std::shared_ptr<dae::GameObject> pPooka{ std::make_shared<dae::GameObject>() };

	// Add components
	// --------------

	// Add animationComponent
	dae::AnimationComponent* pObjectTexture{ pPooka->AddComponent<dae::AnimationComponent>() };
	pObjectTexture->CenterTexture(true);

	// Add enemyComponent
	EnemyComponent* pEnemyComponent{ pPooka->AddComponent<EnemyComponent>() };
	
	enemy::BehaviorData behaviorData{};
	behaviorData.movementSpeed = 100.f;
	
	behaviorData.detectionRange = 2;
	behaviorData.detectionInterval = 1.f;

	const int maxGhostTime{ 10 };
	const int minGhostTime{ 5 };
	behaviorData.ghostTime = static_cast<float>(utils::RandomInteger(minGhostTime, maxGhostTime));
	behaviorData.maxGhostTime = 1.f;

	behaviorData.enemyType = enemy::Pooka;

	pEnemyComponent->SetBehaviorData(behaviorData);
	pEnemyComponent->StoreParent(GetGameObject());

	// Add as child
	// ------------
	pPooka->SetWorldPosition(position);
	pPooka->SetParent(GetGameObject(), true);

	m_pEnemies.emplace_back(pEnemyComponent);
}
void EnemyManager::SpawnFygar(const glm::vec3& position)
{
	// Create gameObject
	// -----------------
	std::shared_ptr<dae::GameObject> pFygar{ std::make_shared<dae::GameObject>() };

	// Add components
	// --------------

	// Add animationComponent
	dae::AnimationComponent* pObjectTexture{ pFygar->AddComponent<dae::AnimationComponent>() };
	pObjectTexture->CenterTexture(true);

	// Add enemyComponent
	EnemyComponent* pEnemyComponent{ pFygar->AddComponent<EnemyComponent>() };

	enemy::BehaviorData behaviorData{};
	behaviorData.movementSpeed = 75.f;

	behaviorData.detectionRange = 1;
	behaviorData.detectionInterval = 1.f;

	int maxTime{ 3 };
	int minTime{ 7 };
	behaviorData.ghostTime = static_cast<float>(utils::RandomInteger(minTime, maxTime));
	behaviorData.maxGhostTime = 2.5f;

	maxTime = 10;
	minTime = 5;
	behaviorData.attackTime = static_cast<float>(utils::RandomInteger(minTime, maxTime));
	behaviorData.attackCharge = 0.5f;
	behaviorData.attackStun = 0.5f;

	behaviorData.enemyType = enemy::Fygar;

	pEnemyComponent->SetBehaviorData(behaviorData);
	pEnemyComponent->StoreParent(GetGameObject());

	// Add as child
	// ------------
	pFygar->SetWorldPosition(position);
	pFygar->SetParent(GetGameObject(), true);

	m_pEnemies.emplace_back(pEnemyComponent);
}