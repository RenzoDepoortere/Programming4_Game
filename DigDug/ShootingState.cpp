#include "ShootingState.h"

#include "CharacterComponent.h"
#include "AnimationComponent.h"
#include "GameObject.h"

#include "ResourceManager.h"

Player::ShootingState::ShootingState()
{
	// Player texture
	// --------------
	std::string textureString{ "Sprites/Characters/MainCharacter/Shoot_Stance.png" };
	m_pShootingSprite = dae::ResourceManager::GetInstance().LoadTexture(textureString);

	// Create rope
	// -----------


	// Shooting texture
	textureString = "Sprites/Characters/MainCharacter/Rope.png";
	m_pRopeSprite = dae::ResourceManager::GetInstance().LoadTexture(textureString);
}

void Player::ShootingState::OnEnter(CharacterComponent* pPlayer)
{
	// Set playerTexture
	auto pAnimationComponent{ pPlayer->GetAnimationComponent() };
	pAnimationComponent->SetTexture(m_pShootingSprite);
	pAnimationComponent->SetPaused(true);

	// Set rope active

}
void Player::ShootingState::OnLeave(CharacterComponent* /*pPlayer*/)
{
}

Player::PlayerStates Player::ShootingState::Update(CharacterComponent* /*pPlayer*/, float /*deltaTime*/)
{


	// Return
	return NR_STATES;
}