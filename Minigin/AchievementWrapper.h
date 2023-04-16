#pragma once
#include "SteamAchievements.h"
#include "Singleton.h"

#include <memory>
#include <string>
#include <array>

namespace dae
{
	class AchievementWrapper final : public Singleton<AchievementWrapper>
	{
	public:
		// Destructor and rule of five
		~AchievementWrapper() = default;

		AchievementWrapper(const AchievementWrapper& other) = delete;
		AchievementWrapper(AchievementWrapper&& other) = delete;
		AchievementWrapper& operator=(const AchievementWrapper& other) = delete;
		AchievementWrapper& operator=(AchievementWrapper&& other) = delete;

		// Public Functions
		void Initialize();
		void SetAchievement(const std::string& achievementID);

	private:
		friend class Singleton<AchievementWrapper>;
		explicit AchievementWrapper() = default;

		// Enum classes
		// ------------
		
		// Defining our achievements
		enum EAchievements
		{
			ACH_WIN_ONE_GAME = 0,
			ACH_WIN_100_GAMES = 1,
			ACH_TRAVEL_FAR_ACCUM = 2,
			ACH_TRAVEL_FAR_SINGLE = 3,
			NR_ACHIEVEMENTS = 4
		};

		// Member Variables
		// ----------------
		std::unique_ptr<steam::CSteamAchievements> m_pSteamAchievements{};
		std::array<steam::Achievement_t, NR_ACHIEVEMENTS> m_SteamAchievements{};


		// Member Functions
		// ----------------
	};
}