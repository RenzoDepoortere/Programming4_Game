#include <stdexcept>
#include <SDL_ttf.h>

#include "FPSCounter.h"
#include "GameObject.h"
#include "TextComponent.h"

dae::FPSCounter::FPSCounter(dae::GameObject* pParentObject)
	: Component{ pParentObject }
	, m_MaxCalculateTime{ 1.f }
{
	m_ElapsedTime = m_MaxCalculateTime;

	// Get component
	m_pTextComponent = GetGameObject()->GetComponent<TextComponent>();
	if (m_pTextComponent == nullptr) std::cout << "Error: no textComponent found" << std::endl;
}

void dae::FPSCounter::Update(float deltaTime)
{
	if (m_pTextComponent == nullptr) return;

	// Count up
	++m_FrameCounter;
	m_ElapsedTime += deltaTime;
	
	// After each second
	if (m_ElapsedTime >= m_MaxCalculateTime)
	{
		m_ElapsedTime -= m_MaxCalculateTime;

		// Store FPS
		const std::string currentFPS{ std::to_string(m_FrameCounter) };

		// Reset frameCounter
		m_FrameCounter = 0;

		// Tell component to show on screen
		m_pTextComponent->SetText(currentFPS);
	}
}