#include "SelectionComponent.h"

#include "DigDugSceneManager.h"

#include "InputManager.h"

SelectionComponent::SelectionComponent(dae::GameObject* pParentObject)
	: Component{ pParentObject }
	, m_XPos{ 180 }
{
}

void SelectionComponent::Update(float /*deltaTime*/)
{
	// If ENTER pressed
	if (dae::InputManager::GetInstance().IsPressed(SDL_SCANCODE_RETURN))
	{
		// Check where arrow is

		// Execute that thing
		digdug::DigDugSceneManager::GetInstance().NextLevel();
	}
}