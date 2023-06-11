#pragma once
#include "PlayerManager.h"
#include "EnemyManager.h"

#include <vector>
#include <string>

namespace dae
{
	class GameObject;
	class Scene;
}
namespace grid
{
	class GridComponent;
}

class CharacterComponent;

namespace digdug
{
	class DigDugScene final
	{
	public:
		// Rule of five
		DigDugScene(dae::Scene* pScene);
		~DigDugScene() = default;

		DigDugScene(const DigDugScene& other) = delete;
		DigDugScene(DigDugScene&& other) = delete;
		DigDugScene& operator=(const DigDugScene& other) = delete;
		DigDugScene& operator=(DigDugScene&& other) = delete;

		// Functionality
		void SetLevel(const std::string& levelPath, unsigned int gameMode);
		void SetActive(bool isActive);
		void Reset(bool deleteObjects);

		grid::GridComponent* GetGrid() const  { return m_pGrid; }
		const std::vector<CharacterComponent*>& GetCharacters() const  { return m_pPlayerManager->GetCharacters(); }
		const std::vector<EnemyComponent*>& GetEnemies() const  { return m_pEnemyManager->GetEnemies(); }

	private:

		// Member Variables
		// ----------------
		dae::GameObject* m_pSceneRootObject{ nullptr };

		grid::GridComponent* m_pGrid{ nullptr };
		PlayerManager* m_pPlayerManager{ nullptr };
		EnemyManager* m_pEnemyManager{ nullptr };

		std::string m_LevelName{};
		int m_NrPlayers{};
		bool m_ControlFygar{};

		// Member Functions
		// ----------------
		void Grid();
		void MainCharacter();
		void Enemies();
	};
}