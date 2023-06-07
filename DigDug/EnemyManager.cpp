#include "EnemyManager.h"

#include "GameObject.h"

#include "GridComponent.h"
#include "AnimationComponent.h"	
#include "EnemyComponent.h"
#include "CharacterComponent.h"

#include "ResourceManager.h"

EnemyManager::EnemyManager(dae::GameObject* pParentObject)
	: Component{ pParentObject }
{
	// Get enemy textures
	m_pPookaTexture = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Characters/Enemies/Pooka/Pooka_Walk_Animation.png");
	m_pFygarTexture = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Characters/Enemies/Fygar/Fygar.png");
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
	pObjectTexture->SetTexture(m_pPookaTexture);

	pObjectTexture->SetSingleSpriteSize(25.f);
	pObjectTexture->SetMaxFrames(2);
	pObjectTexture->SetFramesPerSecond(12);

	// Add enemyComponent
	EnemyComponent* pEnemyComponent{ pPooka->AddComponent<EnemyComponent>() };
	
	enemy::BehaviorData behaviorData{};
	behaviorData.movementSpeed = 100.f;
	
	behaviorData.detectionRange = 2;
	behaviorData.detectionInterval = 1.f;

	behaviorData.enemyType = enemy::Pooka;

	pEnemyComponent->SetBehaviorData(behaviorData);
	pEnemyComponent->SetAnimationComponent(pObjectTexture);
	pEnemyComponent->SetGrid(m_pGrid);
	pEnemyComponent->SetCharacters(m_pCharacters);

	// Add as child
	// ------------
	pPooka->SetWorldPosition(position);
	pPooka->SetParent(GetGameObject(), true);

	m_pEnemies.emplace_back(pEnemyComponent);
}
void EnemyManager::SpawnFygar(const glm::vec3& /*position*/)
{

}