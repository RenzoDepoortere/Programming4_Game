#include "CharacterComponent.h"

#include "AnimationComponent.h"
#include "GameObject.h"
#include "DiggingState.h"
#include "ShootingState.h"
#include "BlowingState.h"
#include "PlayerSquashedState.h"

#include "Renderer.h"

CharacterComponent::CharacterComponent(dae::GameObject* pParentObject)
	: Component{ pParentObject }
{

}

void CharacterComponent::Update(float deltaTime)
{
	// Init states if necessary
	if (m_InitializedStates == false)
	{
		m_InitializedStates = true;
		InitStates();
	}

	// Update currentState
	player::PlayerStates state{};
	state = m_pCurrentState->Update(this, deltaTime);

	// Change state if asked
	if (state != player::NR_STATES)
	{
		m_pCurrentState->OnLeave(this);

		m_pCurrentState = m_pPlayerStates[static_cast<int>(state)].get();
		m_pCurrentState->OnEnter(this);
	}
}
void CharacterComponent::Render() const
{
	//const glm::vec3 worldPos{ GetGameObject()->GetWorldPosition() };
	//const utils::Rect boundingRect{ m_pAnimationComponent->GetBoundingRect() };

	//// Draw boundingRect
	//auto pRenderer{ dae::Renderer::GetInstance().GetSDLRenderer() };
	//SDL_SetRenderDrawColor(pRenderer, static_cast<Uint8>(0), static_cast<Uint8>(0), static_cast<Uint8>(255), static_cast<Uint8>(255));

	//SDL_Rect rect{};
	//rect.x = static_cast<int>(boundingRect.x);
	//rect.y = static_cast<int>(boundingRect.y);
	//rect.w = static_cast<int>(boundingRect.width);
	//rect.h = static_cast<int>(boundingRect.height);

	//SDL_RenderDrawRect(pRenderer, &rect);
}

void CharacterComponent::SetSquashed()
{
	m_pCurrentState->OnLeave(this);

	m_pCurrentState = m_pPlayerStates[static_cast<int>(player::Squashed)].get();
	m_pCurrentState->OnEnter(this);
}

// Only gets called when got hit by rock 
void CharacterComponent::HandleEvent(unsigned int /*eventID*/)
{
	//// Play squashed SFX
	//const int volume{ 100 };
	//const int loops{ 0 };

	//dae::ServiceLocator::GetSoundSystem().PlayAudio(event::RockBreak, volume, loops);
}
void CharacterComponent::OnSubjectDestroy()
{
}

void CharacterComponent::SetLookingDirection(player::LookingDirection lookingDirection)
{
	// Rotate accordingly
	auto pObject{ GetGameObject() };
	switch (lookingDirection)
	{
	case player::Up:
		pObject->SetRotation(90.f);
		m_pAnimationComponent->SetFlip(true);
		break;

	case player::Down:
		pObject->SetRotation(90.f);
		m_pAnimationComponent->SetFlip(false);
		break;

	case player::Left:
		pObject->SetRotation(0.f);
		m_pAnimationComponent->SetFlip(true);
		break;

	case player::Right:
		pObject->SetRotation(0.f);
		m_pAnimationComponent->SetFlip(false);
		break;
	}

	// Set lookingDirection
	m_CurrentLookingDirection = lookingDirection;
}

void CharacterComponent::InitStates()
{
	// Create states
	m_pPlayerStates[static_cast<int>(player::Digging)] = std::make_unique<player::DiggingState>();
	m_pPlayerStates[static_cast<int>(player::Shooting)] = std::make_unique<player::ShootingState>();
	m_pPlayerStates[static_cast<int>(player::Blowing)] = std::make_unique<player::BlowingState>();
	m_pPlayerStates[static_cast<int>(player::Squashed)] = std::make_unique<player::PlayerSquashedState>();

	// Set default state
	m_pCurrentState = m_pPlayerStates[static_cast<int>(player::Digging)].get();
	m_pCurrentState->OnEnter(this);
}