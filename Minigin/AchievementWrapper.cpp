#include "AchievementWrapper.h"

void dae::AchievementWrapper::Initialize()
{
	// Achievement array which will hold data about the achievements and their state
	m_SteamAchievements[0] = _ACH_ID(ACH_WIN_ONE_GAME, "Winner");
	m_SteamAchievements[1] = _ACH_ID(ACH_WIN_100_GAMES, "Champion");
	m_SteamAchievements[2] = _ACH_ID(ACH_TRAVEL_FAR_ACCUM, "Interstellar");
	m_SteamAchievements[3] = _ACH_ID(ACH_TRAVEL_FAR_SINGLE, "Orbiter");

	// Create steamAchievements object
	m_pSteamAchievements = std::make_unique<steam::CSteamAchievements>(m_SteamAchievements.data(), NR_ACHIEVEMENTS);
}

void dae::AchievementWrapper::SetAchievement(const std::string& achievementID)
{
	m_pSteamAchievements->SetAchievement(achievementID.c_str());
}