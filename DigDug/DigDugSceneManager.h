#pragma once
#include "DigDugScene.h"
#include "UIScene.h"
#include "InGameUIScene.h"
#include "Observer.h"

#include <Singleton.h>
#include <vector>
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
	enum Mode
	{
		Single, PvE, PvP
	};

	class DigDugSceneManager final : public dae::Singleton<DigDugSceneManager>, public dae::Observer<>
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
		void NextLevel();
		void ResetLevel() { m_pCurrentScene->Reset(false); }

		void SetGameMode(Mode gameMode) { m_CurrentGameMode = gameMode; }
		const std::vector<unsigned long>& GetControllerIndices() const { return m_ControllerIndices; }

		grid::GridComponent* GetGrid() const { return m_pCurrentScene->GetGrid(); }
		const std::vector<CharacterComponent*>& GetCharacters() const { return m_pCurrentScene->GetCharacters(); }
		const std::vector<EnemyComponent*>& GetEnemies() const { return m_pCurrentScene->GetEnemies(); }

		bool GetIsChangingLevel() const { return m_IsChangingLevel; }

		// Observer
		virtual void HandleEvent(unsigned int eventID) override;
		virtual void OnSubjectDestroy() override;

	private:
		friend class Singleton<DigDugSceneManager>;
		DigDugSceneManager();

		// Member Variables
		// ----------------
		std::unique_ptr<UIScene> m_pUIScene{ nullptr };
		std::unique_ptr<InGameUIScene> m_pInGameUIScene{ nullptr };

		std::unique_ptr<DigDugScene> m_pCurrentScene{ nullptr };
		std::vector<std::string> m_LevelNames{};
		int m_CurrentLevel{};

		bool m_IsChangingLevel{};
		Mode m_CurrentGameMode{};

		std::vector<unsigned long> m_ControllerIndices{};

		// Member Functions
		// ----------------
		void InitSystems();

		void InitInput();
		void KeyboardCommands();
		void ControllerCommand(unsigned long controllerIdx);
		void CommunalCommands();

		void InitMenu(const std::vector<dae::Scene*>& pScenes);
		void InitMainGame(const std::vector<dae::Scene*>& pScenes);
		void InitScenes(const std::vector<dae::Scene*>& pScenes);

		void GoToMenu();
	};
}