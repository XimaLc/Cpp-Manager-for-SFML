#include "StatsAndAchievements.h"
#include <iostream>

/*
	
	Liste des achievements
	
*/
//g_rgAchievements.size()+1 pour l'id 
Achievement_t g_rgAchievements[] =
{
	_ACH_ID(ACH_ENDRUSH, "Aller affronter la Garde du roi sans faire de cycle avant"),
};

/*
	Parametres :
		Aucun
	Retour :
		Aucun
	Action :
		Constructeur
*/
CSteamStatsAndAchievements::CSteamStatsAndAchievements() :
	m_pSteamUserStats(NULL),
	m_GameID(SteamUtils()->GetAppID()),
	m_CallbackUserStatsReceived(this, &CSteamStatsAndAchievements::OnUserStatsReceived),
	m_CallbackUserStatsStored(this, &CSteamStatsAndAchievements::OnUserStatsStored),
	m_CallbackAchievementStored(this, &CSteamStatsAndAchievements::OnAchievementStored)
{
	timer = 0.f;

	m_pSteamUser = SteamUser();
	m_pSteamUserStats = SteamUserStats();
	

	m_bRequestedStats = false;
	m_bStatsValid = false;
	m_bStoreStats = false;

	m_nStats["NumKillMeatmud"] = 0;
	m_nStats["NumKillButcher"] = 0;
	m_nStats["NumKillDavid"] = 0;
	m_nStats["NumKillGuard"] = 0;
	m_nStats["NumKillWalker"] = 0;
	m_nStats["NumKillCrawler"] = 0;
	m_nStats["NumKillSpitter"] = 0;
	m_nStats["NumKillTrapper"] = 0;
	m_nStats["NumKillWatcher"] = 0;
	m_nStats["NumKillBat"] = 0;
	m_nStats["NumKillSkullburn"] = 0;
	m_nStats["NumKillMortarer"] = 0;
	m_nStats["NumFinishedLoop"] = 0;
	m_nStats["NoDeathLoopStreak"] = 0;
	m_nStats["shopRoll"] = 0;
	m_nStats["NumDeath"] = 0;
	m_nStats["EndRush"] = 0;
	m_nStats["upItemTo6"] = 0;
	m_nStats["common"] = 0;
	m_nStats["uncommon"] = 0;
	m_nStats["rare"] = 0;
	m_nStats["epic"] = 0;
	m_nStats["legendary"] = 0;
	m_nStats["speedrun"] = 0;

	RequestStats();
}

/*
	Parametres :
		Aucun
	Retour :
		Bool qui dit si l'appel a reussi ou non
	Action :
		appel ISteamUserStats::RequestCurrentStats
*/
bool CSteamStatsAndAchievements::RequestStats()
{
	// Est-ce que Steam est lancé ? Si non, il est impossible d'obtenir des statistiques.
	if (NULL == SteamUserStats() || NULL == SteamUser())
	{
		return false;
	}
	// Est-ce que la personne est connectée ?  Si non, il est impossible d'obtenir des statistiques.
	if (!SteamUser()->BLoggedOn())
	{
		return false;
	}
	// Demander les statistiques du compte.
	return SteamUserStats()->RequestCurrentStats();
}

/*
	Parametres :
		Aucun
	Retour :
		Aucun
	Action :
		Run a frame for the CStatsAndAchievements. does not need to run at full frame rate.
*/
void CSteamStatsAndAchievements::RunFrame()
{
	if (!m_bRequestedStats)
	{
		// Is Steam Loaded? if no, can't get stats, done
		if (NULL == m_pSteamUserStats || NULL == m_pSteamUser)
		{
			m_bRequestedStats = true;
			return;
		}

		// If yes, request our stats
		bool bSuccess = m_pSteamUserStats->RequestCurrentStats();

		// This function should only return false if we weren't logged in, and we already checked that.
		// But handle it being false again anyway, just ask again later.
		m_bRequestedStats = bSuccess;
	}

	if (!m_bStatsValid)
		return;

	//Savoir si on a debloquer les achievements
	for (Achievement_t Ach : g_rgAchievements)
	{
		EvaluateAchievement(Ach);
	}

	//Enregistrer
	StoreStatsIfNecessary();
}

bool CSteamStatsAndAchievements::ResetStats()
{
	if (m_pSteamUserStats)
	{
		m_pSteamUserStats->ResetAllStats(true);
		StoreStatsIfNecessary();

		m_nStats["NumKillMeatmud"] = 0;
		m_nStats["NumKillButcher"] = 0;
		m_nStats["NumKillDavid"] = 0;
		m_nStats["NumKillGuard"] = 0;
		m_nStats["NumKillWalker"] = 0;
		m_nStats["NumKillCrawler"] = 0;
		m_nStats["NumKillSpitter"] = 0;
		m_nStats["NumKillTrapper"] = 0;
		m_nStats["NumKillWatcher"] = 0;
		m_nStats["NumKillBat"] = 0;
		m_nStats["NumKillSkullburn"] = 0;
		m_nStats["NumKillMortarer"] = 0;
		m_nStats["NumFinishedLoop"] = 0;
		m_nStats["NoDeathLoopStreak"] = 0;
		m_nStats["shopRoll"] = 0;
		m_nStats["NumDeath"] = 0;
		m_nStats["EndRush"] = 0;
		m_nStats["upItemTo6"] = 0;
		m_nStats["common"] = 0;
		m_nStats["uncommon"] = 0;
		m_nStats["rare"] = 0;
		m_nStats["epic"] = 0;
		m_nStats["legendary"] = 0;
		m_nStats["speedrun"] = 0;

		return true;
	}
	else
		return false;
}

/*
	Parametres:
		std::string StatName : Nom de la stat qu'on veut modifier
		int : valeur que l'on veut ajouter a la stat
	Retour:
		Aucun
	Action:
		Ajouter une valeur a une stat dans la map m_nStats
*/
void CSteamStatsAndAchievements::addStat(std::string StatName, int value)
{
	m_nStats[StatName] += value;
	RunFrame();
}

/*
	Parametres:
		std::string StatName : Nom de la stat qu'on veut modifier
		int value : valeur que l'on veut assigner a la stat
	Retour:
		Aucun
	Action:
		Remplace une stat par une valeur
*/
void CSteamStatsAndAchievements::setStat(std::string StatName, int value)
{
	m_nStats[StatName] = value;
	RunFrame();
}

void CSteamStatsAndAchievements::startTimer()
{
	timer = 0.f;
}

void CSteamStatsAndAchievements::addTime()
{
	timer += Tools::getDeltaTime();
}

/*
	Parametres :
		Achievement_t achievement qu'on veut check
	Retour :
		Aucun
	Action :
		Voir si on peut debloquer l'achievement
*/
void CSteamStatsAndAchievements::EvaluateAchievement(Achievement_t& achievement)
{
	//Already have it ?
	if (achievement.m_bAchieved)
		return;

	switch (achievement.m_eAchievementID)
	{
	case ACH_KILL_MEATMUD:
		if (m_nStats["NumKillMeatmud"] > 0)
			UnlockAchievement(achievement);
		break;
	case ACH_KILL_BUTCHER:
		if (m_nStats["NumKillButcher"] > 0)
			UnlockAchievement(achievement);
		break;
	case ACH_KILL_DAVID:
		if (m_nStats["NumKillDavid"] > 0)
			UnlockAchievement(achievement);
		break;
	case ACH_KILL_GUARD:
		if (m_nStats["NumKillGuard"] > 0)
			UnlockAchievement(achievement);
		break;
	case ACH_KILL_MEATMUD50:
		if (m_nStats["NumKillMeatmud"] >= 50)
			UnlockAchievement(achievement);
		break;
	case ACH_KILL_BUTCHER50:
		if (m_nStats["NumKillButcher"] >= 50)
			UnlockAchievement(achievement);
		break;
	case ACH_KILL_DAVID50:
		if (m_nStats["NumKillDavid"] >= 50)
			UnlockAchievement(achievement);
		break;
	case ACH_KILL_GUARD50:
		if (m_nStats["NumKillGuard"] >= 50)
			UnlockAchievement(achievement);
		break;
	case ACH_KILL_WALKER10:
		if (m_nStats["NumKillWalker"] >= 10)
			UnlockAchievement(achievement);
		break;
	case ACH_KILL_WALKER100:
		if (m_nStats["NumKillWalker"] >= 100)
			UnlockAchievement(achievement);
		break;
	case ACH_KILL_WALKER1000:
		if (m_nStats["NumKillWalker"] >= 1000)
			UnlockAchievement(achievement);
		break;
	case ACH_KILL_CRAWLER10:
		if (m_nStats["NumKillCrawler"] >= 10)
			UnlockAchievement(achievement);
		break;
	case ACH_KILL_CRAWLER100:
		if (m_nStats["NumKillCrawler"] >= 100)
			UnlockAchievement(achievement);
		break;
	case ACH_KILL_CRAWLER1000:
		if (m_nStats["NumKillCrawler"] >= 1000)
			UnlockAchievement(achievement);
		break;
	case ACH_KILL_SPITTER10:
		if (m_nStats["NumKillSpitter"] >= 10)
			UnlockAchievement(achievement);
		break;
	case ACH_KILL_SPITTER100:
		if (m_nStats["NumKillSpitter"] >= 100)
			UnlockAchievement(achievement);
		break;
	case ACH_KILL_SPITTER1000:
		if (m_nStats["NumKillSpitter"] >= 1000)
			UnlockAchievement(achievement);
		break;
	case ACH_KILL_TRAPPER10:
		if (m_nStats["NumKillTrapper"] >= 10)
			UnlockAchievement(achievement);
		break;
	case ACH_KILL_TRAPPER100:
		if (m_nStats["NumKillTrapper"] >= 100)
			UnlockAchievement(achievement);
		break;
	case ACH_KILL_TRAPPER1000:
		if (m_nStats["NumKillTrapper"] >= 1000)
			UnlockAchievement(achievement);
		break;
	case ACH_KILL_WATCHER10:
		if (m_nStats["NumKillWatcher"] >= 10)
			UnlockAchievement(achievement);
		break;
	case ACH_KILL_WATCHER100:
		if (m_nStats["NumKillWatcher"] >= 100)
			UnlockAchievement(achievement);
		break;
	case ACH_KILL_WATCHER1000:
		if (m_nStats["NumKillWatcher"] >= 1000)
			UnlockAchievement(achievement);
		break;
	case ACH_KILL_BAT10:
		if (m_nStats["NumKillBat"] >= 10)
			UnlockAchievement(achievement);
		break;
	case ACH_KILL_BAT100:
		if (m_nStats["NumKillBat"] >= 100)
			UnlockAchievement(achievement);
		break;
	case ACH_KILL_BAT1000:
		if (m_nStats["NumKillBat"] >= 1000)
			UnlockAchievement(achievement);
		break;
	case ACH_KILL_SKULLBURN10:
		if (m_nStats["NumKillSkullburn"] >= 10)
			UnlockAchievement(achievement);
		break;
	case ACH_KILL_SKULLBURN100:
		if (m_nStats["NumKillSkullburn"] >= 100)
			UnlockAchievement(achievement);
		break;
	case ACH_KILL_SKULLBURN1000:
		if (m_nStats["NumKillSkullburn"] >= 1000)
			UnlockAchievement(achievement);
		break;
	case ACH_KILL_MORTARER10:
		if (m_nStats["NumKillMortarer"] >= 10)
			UnlockAchievement(achievement);
		break;
	case ACH_KILL_MORTARER100:
		if (m_nStats["NumKillMortarer"] >= 100)
			UnlockAchievement(achievement);
		break;
	case ACH_KILL_MORTARER1000:
		if (m_nStats["NumKillMortarer"] >= 1000)
			UnlockAchievement(achievement);
		break;
	case ACH_50_LOOP:
		if (m_nStats["NumFinishedLoop"] >= 50)
			UnlockAchievement(achievement);
		break;
	case ACH_10_LOOP_NODEATH:
		if (m_nStats["NoDeathLoopStreak"] >= 10)
			UnlockAchievement(achievement);
		break;
	case ACH_SHOP_ROLL:
		if (m_nStats["shopRoll"] >= 10)
			UnlockAchievement(achievement);
		break;
	case ACH_100_DEATH:
		if (m_nStats["NumDeath"] >= 100)
			UnlockAchievement(achievement);
		break;
	case ACH_ENDRUSH:
		if (m_nStats["EndRush"] >= 1)
			UnlockAchievement(achievement);
		break;
	case ACH_MAX_ITEM:
		if (m_nStats["upItemTo6"] >= 1)
			UnlockAchievement(achievement);
		break;
	case ACH_FIRST_EPIC:
		if (m_nStats["getEpicItem"] >= 1)
			UnlockAchievement(achievement);
		break;
	case ACH_FIRST_LEGENDARY:
		if (m_nStats["getLegendaryItem"] >= 1)
			UnlockAchievement(achievement);
		break;
	case ACH_ALL_RARITY:
		if (m_nStats["common"] >= 1 && m_nStats["uncommon"] >= 1 && m_nStats["rare"] >= 1 && m_nStats["epic"] >= 1 && m_nStats["legendary"] >= 1)
			UnlockAchievement(achievement);
		break;
	case ACH_SPEEDRUN:
		if (m_nStats["speedrun"] >= 1 && timer <= 900)
			UnlockAchievement(achievement);
		else
			m_nStats["speedrun"] = 0;
		break;
	}
}

/*
	Parametres :
		Achievement_t & achievement a débloquer
	Retour :
		Aucun
	Action :
		Debloquer l'achievement en parametre
*/
void CSteamStatsAndAchievements::UnlockAchievement(Achievement_t& achievement)
{
	achievement.m_bAchieved = true;
	achievement.m_iIconImage = 0;
	m_pSteamUserStats->SetAchievement(achievement.m_pchAchievementID);
	m_bStoreStats = true;
}

/*
	Parametres :
		Aucun
	Retour :
		Aucun
	Action :
		Enregistre dans la DB Steam
*/
void CSteamStatsAndAchievements::StoreStatsIfNecessary()
{
	if (m_bStoreStats)
	{
		m_pSteamUserStats->SetStat("NumKillMeatmud", m_nStats["NumKillMeatmud"]);
		m_pSteamUserStats->SetStat("NumKillButcher", m_nStats["NumKillButcher"]);
		m_pSteamUserStats->SetStat("NumKillDavid", m_nStats["NumKillDavid"]);
		m_pSteamUserStats->SetStat("NumKillGuard", m_nStats["NumKillGuard"]);
		m_pSteamUserStats->SetStat("NumKillWalker", m_nStats["NumKillWalker"]);
		m_pSteamUserStats->SetStat("NumKillCrawler", m_nStats["NumKillCrawler"]);
		m_pSteamUserStats->SetStat("NumKillSpitter", m_nStats["NumKillSpitter"]);
		m_pSteamUserStats->SetStat("NumKillTrapper", m_nStats["NumKillTrapper"]);
		m_pSteamUserStats->SetStat("NumKillWatcher", m_nStats["NumKillWatcher"]);
		m_pSteamUserStats->SetStat("NumKillBat", m_nStats["NumKillBat"]);
		m_pSteamUserStats->SetStat("NumKillSkullburn", m_nStats["NumKillSkullburn"]);
		m_pSteamUserStats->SetStat("NumKillMortarer", m_nStats["NumKillMortarer"]);
		m_pSteamUserStats->SetStat("NumFinishedLoop", m_nStats["NumFinishedLoop"]);
		m_pSteamUserStats->SetStat("NoDeathLoopStreak", m_nStats["NoDeathLoopStreak"]);
		m_pSteamUserStats->SetStat("ShopRoll", m_nStats["ShopRoll"]);
		m_pSteamUserStats->SetStat("NumDeath", m_nStats["NumDeath"]);
		m_pSteamUserStats->SetStat("EndRush", m_nStats["EndRush"]);

		bool bSuccess = m_pSteamUserStats->StoreStats();

		m_bStoreStats = !bSuccess;
	}
}

/*
	Parametres :
		UserStatsReceived_t *pCallback
	Retour :
		Aucun
	Action :
		We have stats data from Steam. It is authoritative, so update our data with those results now.
*/
void CSteamStatsAndAchievements::OnUserStatsReceived(UserStatsReceived_t* pCallback)
{
	if (!m_pSteamUserStats)
		return;

	// we may get callbacks for other games' stats arriving, ignore them
	if (m_GameID.ToUint64() == pCallback->m_nGameID)
	{
		if (k_EResultOK == pCallback->m_eResult)
		{
			std::cout << "Received stats and achievements from Steam" << std::endl;

			m_bStatsValid = true;

			// load achievements
			for (Achievement_t Ach : g_rgAchievements)
			{
				m_pSteamUserStats->GetAchievement(Ach.m_pchAchievementID, &Ach.m_bAchieved);
				sprintf(Ach.m_rgchName, "%s",
					m_pSteamUserStats->GetAchievementDisplayAttribute(Ach.m_pchAchievementID, "name"));
				sprintf(Ach.m_rgchDescription, "%s",
					m_pSteamUserStats->GetAchievementDisplayAttribute(Ach.m_pchAchievementID, "desc"));
			}

			// load stats
			for (auto i : m_nStats)
			{
				m_pSteamUserStats->GetStat(i.first.c_str(), &i.second);
			}
			//m_pSteamUserStats->GetStat("NumKillButcher", );
			//m_pSteamUserStats->GetStat("NumKillDavid", &m_nStats["NumKillDavid"]);
			//m_pSteamUserStats->GetStat("NumKillGuard", &m_nStats["NumKillGuard"]);
			//m_pSteamUserStats->GetStat("NumKillWalker", &m_nStats["NumKillWalker"]);
			//m_pSteamUserStats->GetStat("NumKillCrawler", &m_nStats["NumKillCrawler"]);
			//m_pSteamUserStats->GetStat("NumKillSpitter", &m_nStats["NumKillSpitter"]);
			//m_pSteamUserStats->GetStat("NumKillTrapper", &m_nStats["NumKillTrapper"]);
			//m_pSteamUserStats->GetStat("NumKillWatcher", &m_nStats["NumKillWatcher"]);
			//m_pSteamUserStats->GetStat("NumKillBat", &m_nStats["NumKillBat"]);
			//m_pSteamUserStats->GetStat("NumKillSkullburn", &m_nStats["NumKillSkullburn"]);
			//m_pSteamUserStats->GetStat("NumKillMortarer", &m_nStats["NumKillMortarer"]);
			//m_pSteamUserStats->GetStat("NumFinishedLoop", &m_nStats["NumFinishedLoop"]);
			//m_pSteamUserStats->GetStat("NoDeathLoopStreak", &m_nStats["NoDeathLoopStreak"]);
			//m_pSteamUserStats->GetStat("ShopRoll", &m_nStats["shopRoll"]);
			//m_pSteamUserStats->GetStat("NumDeath", &m_nStats["NumDeath"]);
			//m_pSteamUserStats->GetStat("EndRush", &m_nStats["EndRush"]);

		}
		else
		{
			char buffer[128];
			sprintf(buffer, "RequestStats - failed, %d\n", pCallback->m_eResult);
			buffer[sizeof(buffer) - 1] = 0;
			std::cout << buffer << std::endl;
		}
	}
}

/*
	Parametres :
		UserStatsStored_t *pCallback
	Retour :
		Aucun
	Action :
		Our stats data was stored!
*/
void CSteamStatsAndAchievements::OnUserStatsStored(UserStatsStored_t* pCallback)
{
	// we may get callbacks for other games' stats arriving, ignore them
	if (m_GameID.ToUint64() == pCallback->m_nGameID)
	{
		if (k_EResultOK == pCallback->m_eResult)
		{
			//std::cout << "StoreStats - success" << std::endl;
		}
		else if (k_EResultInvalidParam == pCallback->m_eResult)
		{
			// One or more stats we set broke a constraint. They've been reverted,
			// and we should re-iterate the values now to keep in sync.
			std::cout << "StoreStats - some failed to validate" << std::endl;
			// Fake up a callback here so that we re-load the values.
			UserStatsReceived_t callback;
			callback.m_eResult = k_EResultOK;
			callback.m_nGameID = m_GameID.ToUint64();
			OnUserStatsReceived(&callback);
		}
		else
		{
			char buffer[128];
			sprintf(buffer, "StoreStats - failed, %d\n", pCallback->m_eResult);
			buffer[sizeof(buffer) - 1] = 0;
			std::cout << buffer << std::endl;
		}
	}
}

/*
	Parametres :
		UserAchievementStored_t* pCallback
	Retour :
		Aucun
	Action :
		An achievement was stored
*/
void CSteamStatsAndAchievements::OnAchievementStored(UserAchievementStored_t* pCallback)
{
	// we may get callbacks for other games' stats arriving, ignore them
	if (m_GameID.ToUint64() == pCallback->m_nGameID)
	{
		if (0 == pCallback->m_nMaxProgress)
		{
			char buffer[128];
			sprintf(buffer, "Achievement '%s' unlocked!", pCallback->m_rgchAchievementName);
			buffer[sizeof(buffer) - 1] = 0;
			std::cout << buffer << std::endl;
		}
		else
		{
			char buffer[128];
			sprintf(buffer, "Achievement '%s' progress callback, (%d,%d)\n",
				pCallback->m_rgchAchievementName, pCallback->m_nCurProgress, pCallback->m_nMaxProgress);
			buffer[sizeof(buffer) - 1] = 0;
			std::cout << buffer << std::endl;
		}
	}
}