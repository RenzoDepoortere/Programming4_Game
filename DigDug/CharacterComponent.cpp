#include "CharacterComponent.h"
#include "GridComponent.h"
#include "GameObject.h"

CharacterComponent::CharacterComponent(dae::GameObject* pParentObject)
	: Component{ pParentObject }
{

}

void CharacterComponent::Update(float /*deltaTime*/)
{
	RemoveDirt();
}

void CharacterComponent::RemoveDirt()
{
	if (m_pGrid == nullptr) return;

	// Get currentCell
	const glm::vec3 objectPos{ GetGameObject()->GetWorldPosition() };
	grid::Cell* pCurrentCell{ m_pGrid->GetCell(objectPos) };

	// Remove texture
	pCurrentCell->textureID = 0;
}