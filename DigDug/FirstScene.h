#pragma once
#include "DigDugScene.h"

#include <vector>

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
	class FirstScene final : public DigDugScene
	{
	public:
		// Rule of five
		FirstScene(dae::Scene* pScene);
		~FirstScene() = default;

		FirstScene(const FirstScene& other) = delete;
		FirstScene(FirstScene&& other) = delete;
		FirstScene& operator=(const FirstScene& other) = delete;
		FirstScene& operator=(FirstScene&& other) = delete;

		// Functionality
		virtual void SetActive(bool isActive);

		virtual grid::GridComponent* GetGrid() const override { return m_pGrid; }
		virtual const std::vector<CharacterComponent*>& GetCharacters() const override { return m_pCharacters; }
		virtual const std::vector<EnemyComponent*>& GetEnemies() const override { return m_pEnemies; }

	private:

		// Member Variables
		// ----------------
		dae::GameObject* m_pSceneRootObject{ nullptr };

		grid::GridComponent* m_pGrid{};
		std::vector<CharacterComponent*> m_pCharacters{};
		std::vector<EnemyComponent*> m_pEnemies{};

		unsigned long m_ControllerIdx{};

		// Member Functions
		// ----------------
		void Grid();
		void MainCharacter();
		void Enemies();
	};
}