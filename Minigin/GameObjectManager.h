#pragma once
#include "Singleton.h"
#include "Scene.h"

namespace dae
{
	class GameObject;

	class GameObjectManager final : public Singleton<GameObjectManager>
	{
	public:
		// Destructor and rule of five
		~GameObjectManager() = default;

		GameObjectManager(const GameObjectManager& other) = delete;
		GameObjectManager(GameObjectManager&& other) = delete;
		GameObjectManager& operator=(const GameObjectManager& other) = delete;
		GameObjectManager& operator=(GameObjectManager&& other) = delete;

		// Public Functions
		void CreateGameObjects(Scene& scene);

	private:
		friend class Singleton<GameObjectManager>;
		explicit GameObjectManager() = default;

		// Member Variables
		// ----------------
		GameObject* m_pMainCharacter{ nullptr };
		GameObject* m_pSideCharacter{ nullptr };

		unsigned long m_ControllerIdx{};

		// Member Functions
		// ----------------
		void FirstWeekObjects(Scene& scene);
		void SecondWeekObjects(Scene& scene);
		void FourthWeekObjects(Scene& scene);
		void FifthWeekObjects(Scene& scene);
		void SixthWeekObjects(Scene& scene);
	};
}