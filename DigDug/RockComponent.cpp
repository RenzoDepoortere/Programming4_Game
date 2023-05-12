#include "RockComponent.h"
#include "GameObject.h"
#include "GridComponent.h"

RockComponent::RockComponent(dae::GameObject* pParentObject)
	: Component{ pParentObject }
{
}

void RockComponent::Update(float deltaTime)
{
	if (m_pGrid == nullptr) return;

	BottomCellGone();
	Rumble(deltaTime);
}

void RockComponent::BottomCellGone()
{
	if (m_CurrentRockState != Passive) return;

	// Get currentCell
	const glm::vec3 currentPos{ GetGameObject()->GetWorldPosition() };
	const grid::Cell* pCurrentCell{ m_pGrid->GetCell(currentPos) };

	// Get cell below
	glm::vec3 bottomCellPos{ currentPos };
	bottomCellPos.y += pCurrentCell->size.y;
	const grid::Cell* pCellBelow{ m_pGrid->GetCell(bottomCellPos) };

	// If cell below is destroyed
	if (pCellBelow->textureID == 0)			// It can be assured that the cell below a rock is never null
	{
		// Current state is unstable
		m_CurrentRockState = Unstable;
	}
}
void RockComponent::Rumble(float /*deltaTime*/)
{
	if (m_CurrentRockState != Passive) return;
}