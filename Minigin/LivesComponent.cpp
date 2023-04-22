#include "LivesComponent.h"
#include "HealthComponent.h"
#include "TextComponent.h"
#include "GameObject.h"

dae::LivesComponent::LivesComponent(dae::GameObject* pParentObject)
	:Component{ pParentObject }
{
	// Get components
	m_pTextComponent = pParentObject->GetComponent<dae::TextComponent>();

	// Default
	m_NrLives = 3;
	m_BaseString = "Lives: ";
}
dae::LivesComponent::~LivesComponent()
{
	if (m_pToFollow) m_pToFollow->GetSubject<>()->RemoveObserver(this);
}
void dae::LivesComponent::OnSubjectDestroy()
{
	m_pToFollow = nullptr;
}

void dae::LivesComponent::HandleEvent()
{
	--m_NrLives;
	if (m_NrLives < 0) m_NrLives = 0;

	UpdateText();
}

void dae::LivesComponent::SetComponentToFollow(dae::HealthComponent* pToFollow)
{
	m_pToFollow = pToFollow;
	m_pToFollow->GetSubject<>()->AddObserver(this);

	UpdateText();
}

void dae::LivesComponent::UpdateText()
{
	m_pTextComponent->SetText(m_BaseString + std::to_string(m_NrLives));
}