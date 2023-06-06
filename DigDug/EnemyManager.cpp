#include "EnemyManager.h"

#include "GameObject.h"

#include "GridComponent.h"
#include "RenderTextureComponent.h"
#include "EnemyComponent.h"

#include "ResourceManager.h"

EnemyManager::EnemyManager(dae::GameObject* pParentObject)
	: Component{ pParentObject }
{
	// Get enemy textures
	m_pPookaTexture = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Characters/Enemies/Pooka/Pooka.png");
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

void EnemyManager::ControlEnemy(unsigned long controllerID, Enemy::EnemyTypes enemyType)
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

void EnemyManager::SpawnPooka(const glm::vec3& position)
{
	// Create gameObject
	// -----------------
	std::shared_ptr<dae::GameObject> pPooka{ std::make_shared<dae::GameObject>() };

	// Add components
	// --------------

	// Add texture
	dae::RenderTextureComponent* pObjectTexture{ pPooka->AddComponent<dae::RenderTextureComponent>() };
	pObjectTexture->CenterTexture(true);
	pObjectTexture->SetTexture(m_pPookaTexture);

	// Add enemyComponent
	EnemyComponent* pEnemyComponent{ pPooka->AddComponent<EnemyComponent>() };
	
	Enemy::BehaviorData behaviorData{};
	behaviorData.movementSpeed = 100.f;
	behaviorData.enemyType = Enemy::Pooka;

	pEnemyComponent->SetBehaviorData(behaviorData);
	pEnemyComponent->SetGrid(m_pGrid);

	// Add as child
	// ------------
	pPooka->SetWorldPosition(position);
	pPooka->SetParent(GetGameObject(), true);

	m_pEnemies.emplace_back(pEnemyComponent);
}

void EnemyManager::SpawnFygar(const glm::vec3& /*position*/)
{

}