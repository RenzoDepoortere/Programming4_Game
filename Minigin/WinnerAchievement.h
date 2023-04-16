#pragma once
#include "Follower.h"

namespace dae
{
	class GameObject;
	class TextComponent;
	class PointComponent;
}

class WinnerAchievement final : public Listener
{
public:
	// Rule of five
	explicit WinnerAchievement() = default;
	virtual ~WinnerAchievement() = default;

	WinnerAchievement(const WinnerAchievement& other) = delete;
	WinnerAchievement(WinnerAchievement&& other) = delete;
	WinnerAchievement& operator=(const WinnerAchievement& other) = delete;
	WinnerAchievement& operator=(WinnerAchievement&& other) = delete;

	// Public functions
	virtual void Notify(const Event& event) override;

private:
	// Member variables
	// ----------------
	bool m_AchievementUnlocked{ false };
};

