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

class CharacterComponent final : public Component, public dae::Observer<void*>
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
	void Reset();
	void StoreParent(dae::GameObject* pParent) { m_pParent = pParent; }

	virtual void Update(float deltaTime) override;
	virtual void Render() const override;

	dae::AnimationComponent* GetAnimationComponent();

	void SetPlayerID(int playerID) { m_PlayerID = playerID; }
	int GetPlayerID() const { return m_PlayerID; }

	void SetLookingDirection(player::LookingDirection lookingDirection);
	player::LookingDirection GetLookingDirection() const { return m_CurrentLookingDirection; }

	void SetCaughtEnemy(EnemyComponent* pEnemy) { m_pCaughtEnemy = pEnemy; }
	EnemyComponent* GetCaughtEnemy() const { return m_pCaughtEnemy; }

	void SetSquashed();
	void SetHit();

	player::PlayerStates GetCurrentStateID() const { return m_CurrentStateID; }

	// Observer
	virtual void HandleEvent(unsigned int eventID, void* extraInfo) override;
	virtual void OnSubjectDestroy() override;

private:
	// Member variables
	// ----------------
	dae::AnimationComponent* m_pAnimationComponent{ nullptr };
	dae::GameObject* m_pParent{ nullptr };

	int m_PlayerID{};

	player::CharacterState* m_pCurrentState{};
	player::PlayerStates m_CurrentStateID{};
	std::array<std::unique_ptr<player::CharacterState>, player::NR_STATES> m_pPlayerStates{};
	bool m_StateInitialized{};

	player::LookingDirection m_CurrentLookingDirection{ player::Right };
	bool m_Died{ false };

	EnemyComponent* m_pCaughtEnemy{ nullptr };

	// Member functions
	// ----------------
	void InitStates();
	void OnInactive();
};

