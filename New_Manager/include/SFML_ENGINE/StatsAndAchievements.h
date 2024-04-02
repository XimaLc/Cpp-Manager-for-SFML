#pragma once
#include "Steam/steam_api.h"
#include <map>
#include <string>
#include "Tools.h"

#define _ACH_ID( id, name ) { id, #id, name, "", 0, 0 }

/*

	Achievements

*/
//Liste des achievements
enum EAchievements
{
	ACH_KILL_MEATMUD = 0,
	ACH_KILL_BUTCHER,
	ACH_KILL_DAVID,
	ACH_KILL_GUARD,
	ACH_KILL_MEATMUD50,
	ACH_KILL_BUTCHER50,
	ACH_KILL_DAVID50,
	ACH_KILL_GUARD50,
	ACH_KILL_WALKER10,
	ACH_KILL_WALKER100,
	ACH_KILL_WALKER1000,
	ACH_KILL_CRAWLER10,
	ACH_KILL_CRAWLER100,
	ACH_KILL_CRAWLER1000,
	ACH_KILL_SPITTER10,
	ACH_KILL_SPITTER100,
	ACH_KILL_SPITTER1000,
	ACH_KILL_TRAPPER10,
	ACH_KILL_TRAPPER100,
	ACH_KILL_TRAPPER1000,
	ACH_KILL_WATCHER10,
	ACH_KILL_WATCHER100,
	ACH_KILL_WATCHER1000,
	ACH_KILL_BAT10,
	ACH_KILL_BAT100,
	ACH_KILL_BAT1000,
	ACH_KILL_SKULLBURN10,
	ACH_KILL_SKULLBURN100,
	ACH_KILL_SKULLBURN1000,
	ACH_KILL_MORTARER10,
	ACH_KILL_MORTARER100,
	ACH_KILL_MORTARER1000,
	ACH_50_LOOP,
	ACH_10_LOOP_NODEATH,
	ACH_ALLRARITY_ONELOOP,
	ACH_MAX_ITEM,
	ACH_FIRST_EPIC,
	ACH_FIRST_LEGENDARY,
	ACH_ALL_BONUSES,
	ACH_ALL_BONUSES_TOGETHER,
	ACH_SHOP_ROLL,
	ACH_100_DEATH,
	ACH_ALL_RARITY,
	ACH_SPEEDRUN,
	ACH_ENDRUSH,
};

struct Achievement_t
{
	int m_eAchievementID;
	const char* m_pchAchievementID;
	char m_rgchName[128];
	char m_rgchDescription[256];
	bool m_bAchieved;
	int m_iIconImage;
};

class CSteamStatsAndAchievements
{
private:
	//Savoir si on a l'achievements mtn
	void EvaluateAchievement(Achievement_t& achievement);
	void UnlockAchievement(Achievement_t& achievement);

	//Request Stats
	bool RequestStats();

	//Store Stats
	void StoreStatsIfNecessary();

	//Game ID
	CGameID m_GameID;

	//Interface utilisateur Steam
	ISteamUser* m_pSteamUser;
	ISteamUserStats* m_pSteamUserStats;

	// On a recu les stats de Steam ?
	bool m_bRequestedStats;
	bool m_bStatsValid;

	//Quelque chose a enregistrer ? 
	bool m_bStoreStats;

	//Persisted Stats details
	std::map<std::string, int> m_nStats;

	float timer;
public:
	//Constructeur
	CSteamStatsAndAchievements();

	void RunFrame();
	bool ResetStats();
	//void Render();
	//void OnGameStateChange();
	//double GetGameDurationSeconds() { return m_flGameDurationSeconds; }

	void addStat(std::string StatName, int value);
	void setStat(std::string StatName, int value);
	void startTimer();
	void addTime();
	
	STEAM_CALLBACK(CSteamStatsAndAchievements, OnUserStatsReceived, UserStatsReceived_t, m_CallbackUserStatsReceived);
	STEAM_CALLBACK(CSteamStatsAndAchievements, OnUserStatsStored, UserStatsStored_t, m_CallbackUserStatsStored);
	STEAM_CALLBACK(CSteamStatsAndAchievements, OnAchievementStored, UserAchievementStored_t, m_CallbackAchievementStored);
};