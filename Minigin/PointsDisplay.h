#pragma once
#include "Follower.h"

namespace dae
{
	class GameObject;
	class TextComponent;
	class PointComponent;
}

class PointsDisplay final : public Observer
{
public:
	// Rule of five
	explicit PointsDisplay(dae::GameObject* pGameObjectToFollow, dae::TextComponent* pTextComponent);
	virtual ~PointsDisplay() = default;

	PointsDisplay(const PointsDisplay& other) = delete;
	PointsDisplay(PointsDisplay&& other) = delete;
	PointsDisplay& operator=(const PointsDisplay& other) = delete;
	PointsDisplay& operator=(PointsDisplay&& other) = delete;

	// Public functions
	virtual void Notify(const Event& event) override;

private:
	// Member variables
	// ----------------

	dae::TextComponent* m_pTextComponent{ nullptr };
	std::string m_BaseString{};
	std::string m_EventName{};

	dae::PointComponent* m_pPointComponent{ nullptr };
};

