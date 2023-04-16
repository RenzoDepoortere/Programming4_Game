#pragma once
#include "Follower.h"

namespace dae
{
	class GameObject;
	class TextComponent;
}

class LivesDisplay final : public Observer
{
public:
	// Rule of five
	explicit LivesDisplay(dae::GameObject* pGameObjectToFollow, dae::TextComponent* pTextComponent);
	virtual ~LivesDisplay() = default;

	LivesDisplay(const LivesDisplay& other) = delete;
	LivesDisplay(LivesDisplay&& other) = delete;
	LivesDisplay& operator=(const LivesDisplay& other) = delete;
	LivesDisplay& operator=(LivesDisplay&& other) = delete;

	// Public functions
	virtual void Notify(const Event& event) override;

private:
	// Member variables
	// ----------------

	dae::TextComponent* m_pTextComponent{ nullptr };
	std::string m_BaseString{};
	std::string m_EventName{};

	int m_MaxLives{};
	int m_CurrentLives{};

	// Member functions
	// ----------------
};

