#pragma once
#include "Component.h"
#include "MoveCommand.h"
#include "CharacterState.h"
#include "Observer.h"

#include <memory>
#include <array>

namespace dae
{
	class GameObject;
	class AnimationComponent;
}

namespace grid
{
	class GridComponent;
}

namespace player
{
	enum LookingDirection
	{
		Up, Down, Left, Right
	};
}

class EnemyManager;
class EnemyComponent;

class CharacterComponent final : public Component, public dae::Observer<>
{
public:
	// Rule of five
	explicit CharacterComponent(dae::GameObject* pParentObject);
	virtual ~CharacterComponent() override = default;

	CharacterComponent(const CharacterComponent& other) = delete;
	CharacterComponent(CharacterComponent&& other) = delete;
	CharacterComponent& operator=(const CharacterComponent& other) = delete;
	CharacterComponent& operator=(CharacterComponent&& other) = delete;

	// Functionality
	virtual void Update(float deltaTime) override;
	virtual void Render() const override;

	void SetGrid(grid::GridComponent* pGrid) { m_pGrid = pGrid; }
	grid::GridComponent* GetGrid() const { return m_pGrid; }

	void SetEnemyManager(EnemyManager* pEnemyManager) { m_pEnemyManager = pEnemyManager; }
	EnemyManager* GetEnemyManager() const { return m_pEnemyManager; }

	void SetAnimationComponent(dae::AnimationComponent* pAnimationComponent) { m_pAnimationComponent = pAnimationComponent; }
	dae::AnimationComponent* GetAnimationComponent() const { return m_pAnimationComponent; }

	void SetControllerID(unsigned long controllerID) { m_ControllerID = controllerID; }

	void SetLookingDirection(player::LookingDirection lookingDirection);
	player::LookingDirection GetLookingDirection() const { return m_CurrentLookingDirection; }

	void SetCaughtEnemy(EnemyComponent* pEnemy) { m_pCaughtEnemy = pEnemy; }
	EnemyComponent* GetCaughtEnemy() const { return m_pCaughtEnemy; }

	void SetSquashed();

	// Observer
	virtual void HandleEvent(unsigned int eventID) override;
	virtual void OnSubjectDestroy() override;

private:
	// Member variables
	// ----------------
	grid::GridComponent* m_pGrid{ nullptr };
	EnemyManager* m_pEnemyManager{ nullptr };
	dae::AnimationComponent* m_pAnimationComponent{ nullptr };

	unsigned long m_ControllerID{};

	player::CharacterState* m_pCurrentState{};
	std::array<std::unique_ptr<player::CharacterState>, player::NR_STATES> m_pPlayerStates{};
	bool m_InitializedStates{ false };

	player::LookingDirection m_CurrentLookingDirection{ player::Right };

	EnemyComponent* m_pCaughtEnemy{ nullptr };

	// Member functions
	// ----------------
	void InitStates();
};

