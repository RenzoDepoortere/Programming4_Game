#include "WinnerAchievement.h"
#include "AchievementWrapper.h"

void WinnerAchievement::Notify(const Event& event)
{
	const int nrPoints{ std::any_cast<int>(event.extraArguments[0]) };
	const int pointsTreshold{ 500 };

	// If reached more then 500 points and haven't unlocked yet
	if (nrPoints >= pointsTreshold && m_AchievementUnlocked == false)
	{
		dae::AchievementWrapper::GetInstance().SetAchievement("ACH_WIN_100_GAMES");
		m_AchievementUnlocked = true;
	}
}