#include "EnemyManager.h"

#include "GameObject.h"
#include "GridComponent.h"
#include "AnimationComponent.h"	
#include "EnemyComponent.h"
#include "CharacterComponent.h"

#include "ResourceManager.h"
#include "EventManager.h"
#include "EventsEnum.h"

#include <algorithm>

EnemyManager::EnemyManager(dae::GameObject* pParentObject)
	: Component{ pParentObject }
{
	// Subscribe to enemyDeath event
	dae::EventManager<grid::Cell*, bool>::GetInstance().Subscribe(event::EnemyDeath, this);
}

void EnemyManager::SpawnEnemies()
{
	// Loop through data
	for (const auto& currentData : m_pGrid->GetEnemySpawnData())
	{
		// If ID is 0, spawn Pooka
		if (currentData.second == 0) SpawnPooka(currentData.first);
		// Else, spawn Fygar
		else						 SpawnFygar(currentData.first);
	}
}

void EnemyManager::ControlEnemy(unsigned long controllerID, enemy::EnemyTypes enemyType)
{
	// Loop through enemies
	for (const auto& currentEnemy : m_pEnemies)
	{
		// Check if enemy is desired type
		if (currentEnemy->GetBehaviorData().enemyType == enemyType)
		{
			// Set control of enemy to controller
			currentEnemy->SetControl(controllerID);
			return;
		}
	}

	// State that enemyType was not found
	std::cout << "Error: Requested enemyType not found" << std::endl;
}

void EnemyManager::SetCharacters(const std::vector<CharacterComponent*>& pCharacters)
{
	m_pCharacters = pCharacters;
	for (const auto& currentCharacter : m_pCharacters)
	{
		currentCharacter->SetEnemyManager(this);
	}
}

bool EnemyManager::CollidesEnemy(const glm::vec3 position, EnemyComponent*& pEnemy) const
{
	// Loop through enemies
	for (const auto& currentEnemy : m_pEnemies)
	{
		// Check if point in enemy
		if (currentEnemy->IsInsideEnemy(position))
		{
			pEnemy = currentEnemy;
			return true;
		}
	}

	return false;
}

void EnemyManager::HandleEvent(unsigned int /*eventID*/, grid::Cell* /*pCell*/, bool /*wasSquashed*/)
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
	pEnemyComponent->SetAnimationComponent(pObjectTexture);
	pEnemyComponent->SetCharacters(m_pCharacters);

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

	const int maxGhostTime{ 15 };
	const int minGhostTime{ 10 };
	behaviorData.ghostTime = static_cast<float>(utils::RandomInteger(minGhostTime, maxGhostTime));
	behaviorData.maxGhostTime = 2.5f;

	behaviorData.enemyType = enemy::Fygar;

	pEnemyComponent->SetBehaviorData(behaviorData);
	pEnemyComponent->SetAnimationComponent(pObjectTexture);
	pEnemyComponent->SetCharacters(m_pCharacters);

	// Add as child
	// ------------
	pFygar->SetWorldPosition(position);
	pFygar->SetParent(GetGameObject(), true);

	m_pEnemies.emplace_back(pEnemyComponent);
}