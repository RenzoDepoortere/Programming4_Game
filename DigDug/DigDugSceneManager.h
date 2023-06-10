#pragma once
#include "DigDugScene.h"

#include <Singleton.h>
#include <vector>
#include <array>
#include <memory>

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
class EnemyComponent;

namespace digdug
{
	class DigDugSceneManager final : public dae::Singleton<DigDugSceneManager>
	{
	public:
		// Destructor and rule of five
		~DigDugSceneManager();

		DigDugSceneManager(const DigDugSceneManager& other) = delete;
		DigDugSceneManager(DigDugSceneManager&& other) = delete;
		DigDugSceneManager& operator=(const DigDugSceneManager& other) = delete;
		DigDugSceneManager& operator=(DigDugSceneManager&& other) = delete;

		// Functionality
		void Initialize(const std::vector<dae::Scene*>& pScenes);
		void SetLevel(int levelIndex);

		grid::GridComponent* GetGrid() const { return m_pCurrentScene->GetGrid(); }
		const std::vector<CharacterComponent*>& GetCharacters() const { return m_pCurrentScene->GetCharacters(); }
		const std::vector<EnemyComponent*>& GetEnemies() const { return m_pCurrentScene->GetEnemies(); }

	private:
		friend class Singleton<DigDugSceneManager>;
		DigDugSceneManager() = default;

		// Member Variables
		// ----------------
		DigDugScene* m_pCurrentScene{ nullptr };
		std::array<std::unique_ptr<DigDugScene>, 3> m_pScenes{};

		unsigned long m_FirstControllerIdx{};
		unsigned long m_SecondControllerIdx{};

		// Member Functions
		// ----------------
		void InitSystems();
		void InitMainGame(const std::vector<dae::Scene*>& pScenes);
		void InitScenes(const std::vector<dae::Scene*>& pScenes);
	};
}