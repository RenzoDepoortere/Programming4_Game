#pragma once
#include "Component.h"
#include "MoveCommand.h"
#include "Subject.h"

#include <vector>

namespace dae
{
	class GameObject;
	class AnimationComponent;
}

namespace grid
{
	class GridComponent;
	struct Cell;
}

class EnemyComponent;
class CharacterComponent;

class RockComponent final : public Component
{
public:
	// Rule of five
	explicit RockComponent(dae::GameObject* pParentObject);
	virtual ~RockComponent() override = default;

	RockComponent(const RockComponent& other) = delete;
	RockComponent(RockComponent&& other) = delete;
	RockComponent& operator=(const RockComponent& other) = delete;
	RockComponent& operator=(RockComponent&& other) = delete;

	// Functionality
	void Reset();

	virtual void Update(float deltaTime) override;
	virtual void Render() const override;

	void SetGrid(grid::GridComponent* pGrid) { m_pGrid = pGrid; }
	void SetAnimationComponent(dae::AnimationComponent* pAnimationComponent) { m_pAnimationComponent = pAnimationComponent; }

	template<typename... Args>
	dae::Subject<void*>* GetSubject() { return &m_HasBeenDestroyed; }

private:
	// Enums
	// -----
	enum RockState
	{
		Passive, Unstable, Falling, Destroyed
	};

	// Member variables
	// ----------------
	grid::GridComponent* m_pGrid{ nullptr };
	dae::AnimationComponent* m_pAnimationComponent{ nullptr };
	RockState m_CurrentRockState{ Passive };

	int m_RotationDirection{ 1 };
	float m_CurrentRumbleTime{};

	std::unique_ptr<dae::MoveCommand> m_pMoveCommand{ nullptr };
	dae::Subject<void*> m_HasBeenDestroyed{};

	// Member functions
	// ----------------
	void BottomCellGone();
	void Rumble(float deltaTime);
	void Fall(float deltaTime);
	void Destroy(float deltaTime);

	grid::Cell* GetCellBelow() const;
	bool CheckIfCanAdd(const utils::Rect& ownBoundingRect, dae::GameObject* pOwnGameObject, const utils::Rect& boundingRect, dae::GameObject* pGameObject);
};

