#include "LivesComponent.h"

#include "GameObject.h"
#include "RenderTextureComponent.h"
#include "Texture2D.h"

#include "ResourceManager.h"

#include <memory>
#include <iostream>

LivesComponent::LivesComponent(dae::GameObject* pParentObject)
	: Component{ pParentObject }
{
}

void LivesComponent::SetMaxNrLives(int maxLives)
{
	// Store variable
	// --------------
	m_MaxLives = maxLives;

	// Create live gameObjects
	// -----------------------

	// Remove children
	GetGameObject()->RemoveAllChildren();
	m_pRenders.clear();

	std::shared_ptr<dae::GameObject> pGameObject{};
	auto pTexture = dae::ResourceManager::GetInstance().LoadTexture("Sprites/Menu/Lives_Display.png");
	
	const int textureSize{ static_cast<int>(pTexture->GetSize().x) };
	const float spaceBetween{ 10 };
	const float startPos{ 640 };
	float xPos{};

	for (int idx{}; idx < m_MaxLives; ++idx)
	{
		// Create gameObject
		pGameObject = std::make_shared<dae::GameObject>();

		// Add components
		auto pTextureComponent = pGameObject->AddComponent<dae::RenderTextureComponent>();
		pTextureComponent->SetTexture(pTexture);

		// Add add child
		pGameObject->SetParent(GetGameObject(), false);
		m_pRenders.emplace_back(pGameObject.get());

		// Set Position
		xPos = startPos - idx * (textureSize + spaceBetween);
		pGameObject->SetWorldPosition(xPos, 260, 0);
	}
}
bool LivesComponent::LoseLive()
{
	// Lose live
	--m_CurrentLives;

	// Hide object
	m_pRenders[m_CurrentLives]->SetIsHidden(true);

	// Return
	if (m_CurrentLives == 0) return true;
	else					 return false;
}
void LivesComponent::ResetLives()
{
	// Reset lives
	m_CurrentLives = m_MaxLives;

	// Unhide objects
	for (const auto& currentRender : m_pRenders)
	{
		currentRender->SetIsHidden(false);
	}
}