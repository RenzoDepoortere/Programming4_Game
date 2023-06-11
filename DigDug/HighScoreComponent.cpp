#include "HighScoreComponent.h"

#include "ResourceManager.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "Texture2D.h"

#include "EventManager.h"
#include "EventsEnum.h"

HighScoreComponent::HighScoreComponent(dae::GameObject* pParentObject)
	: Component{ pParentObject }
{
	// Create textComponent
	// --------------------
	auto font = dae::ResourceManager::GetInstance().LoadFont("Other/Arcade.ttf", 48);
	auto pTexture = std::make_shared<dae::Texture2D>(nullptr);

	m_pTextComponent = GetGameObject()->AddComponent<dae::TextComponent>();
	m_pTextComponent->SetText("0000");
	m_pTextComponent->SetFont(font);
	m_pTextComponent->SetTexture(pTexture);

	// Get highestScore
	// ----------------


	// Subscribe to events
	// -------------------
}
HighScoreComponent::~HighScoreComponent()
{
	// Unsubscribe
}

void HighScoreComponent::HandleEvent(unsigned int /*eventID*/, float newHighScore)
{
	// Set text
	m_pTextComponent->SetText(std::to_string(newHighScore));
}
void HighScoreComponent::OnSubjectDestroy()
{
}