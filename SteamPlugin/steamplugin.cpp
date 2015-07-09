/*
Copyright (c) 2015 Benjamin Moir <bennyboy.private@hotmail.com.au>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "steamplugin.h"
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <steam_api.h>

#pragma comment(lib, "steam_api.lib")

SteamPlugin::SteamPlugin() :
	m_CallbackUserStatsReceived(this, &SteamPlugin::OnUserStatsReceived),
	m_CallbackUserStatsStored(this, &SteamPlugin::OnUserStatsStored),
	m_CallbackAchievementStored(this, &SteamPlugin::OnAchievementStored)
{
	InitSteam();
}

SteamPlugin::~SteamPlugin(void)
{
	SteamAPI_Shutdown();
	m_SteamInitialized = false;
}

bool SteamPlugin::InitSteam()
{
	m_AppId = 0;
	m_StatsInitialized = false;
	m_SteamInitialized = SteamAPI_Init();
	if (!m_SteamInitialized)
		return false;
	m_AppId = SteamUtils()->GetAppID();
	return RequestStats();
}

bool SteamPlugin::Update()
{
	if (m_SteamInitialized)
	{
		SteamAPI_RunCallbacks();
		return true;
	}
	return false;
}

double SteamPlugin::CallMethod(va_list args, const char *method)
{
	double result = false;
	
	if (!strcmp(method, "RequestStats"))
		result = RequestStats();
	else if (!strcmp(method, "SetAchievement"))
	{
		const char *id = va_arg(args, const char *);
		result = SetAchievement(id);
	}
	else if (!strcmp(method, "IsAchieved"))
	{
		const char *id = va_arg(args, const char *);
		result = IsAchieved(id);
	}
	else if (!strcmp(method, "ClearAchievement"))
	{
		const char *id = va_arg(args, const char *);
		result = ClearAchievement(id);
	}
	else if (!strcmp(method, "SetStat"))
	{
		const char *id = va_arg(args, const char *);
		double value   = va_arg(args, double);
		if (value == (int)value)
			result = SetStat(id, (int)value);
		else
			result = SetStat(id, (float)value);
	}
	else if (!strcmp(method, "GetStat"))
	{
		const char *id = va_arg(args, const char *);
		result = GetStat(id);
	}
	else if (!strcmp(method, "GetStatInt"))
	{
		const char *id = va_arg(args, const char *);
		result = (int)GetStat(id);
	}
	else if (!strcmp(method, "GetStatFloat"))
	{
		const char *id = va_arg(args, const char *);
		result = (float)GetStat(id);
	}
	else if (!strcmp(method, "ResetAllStats"))
	{
		bool achievements = !!va_arg(args, double);
		result = ResetAllStats(achievements);
	}
	
	return result;
}

int SteamPlugin::GetProperty(const char *property)
{
	if (!strcmp(property, "SteamAvailable"))
		return m_SteamInitialized;
	else if (!strcmp(property, "StatsAvailable"))
		return m_StatsInitialized;
	else if (!strcmp(property, "NumAchievements"))
		return GetNumAchievements();
	else if (!strcmp(property, "AppId"))
		return m_AppId;
	else if (!strcmp(property, "LoggedOn"))
		return SteamUser()->BLoggedOn();
	return false;
}

bool SteamPlugin::SetProperty(const char *property, double value)
{
	// There are no properties that can really be changed yet.
	return false;
}

void SteamPlugin::OnUserStatsReceived(UserStatsReceived_t *pCallback)
{
	if (pCallback->m_nGameID == m_AppId)
	{
		if (pCallback->m_eResult == k_EResultOK)
			m_StatsInitialized = true;
	}
}

void SteamPlugin::OnUserStatsStored(UserStatsStored_t *pCallback)
{
	// We may get callbacks for other games' stats arriving, ignore them
	if (pCallback->m_nGameID == m_AppId)
	{
		if (pCallback->m_eResult == k_EResultOK)
			std::cout << "Stored stats for Steam\n";
		else
		{
			char buffer[128];
			_snprintf(buffer, 128, "StatsStored - failed, %d\n", pCallback->m_eResult);
			std::cout << buffer;
		}
	}
}

void SteamPlugin::OnAchievementStored(UserAchievementStored_t *pCallback)
{
	// We may get callbacks for other games' stats arriving, ignore them
	if (pCallback->m_nGameID == m_AppId)
		std::cout << "Stored achievement for Steam\n";
}

bool SteamPlugin::RequestStats()
{
	// Is Steam loaded? If not we can't get stats
	if (SteamUserStats() == NULL || SteamUser() == NULL)
		return false;
	
	// Is the user logged on? If not we can't get stats
	if (!SteamUser()->BLoggedOn())
		return false;
	
	// Request user stats
	return SteamUserStats()->RequestCurrentStats();
}

bool SteamPlugin::SetAchievement(const char *id)
{
	if (!m_StatsInitialized)
		return false;
	
	if (!SteamUserStats()->SetAchievement(id))
		return false;
	
	return SteamUserStats()->StoreStats();
}

bool SteamPlugin::IsAchieved(const char *id)
{
	if (!m_StatsInitialized)
		return false;
	
	bool achieved;
	SteamUserStats()->GetAchievement(id, &achieved);
	
	return achieved;
}

bool SteamPlugin::ClearAchievement(const char *id)
{
	if (!m_StatsInitialized)
		return false;
	else
	{
		if (!SteamUserStats()->ClearAchievement(id))
			return false;
		return SteamUserStats()->StoreStats();
	}
}

int SteamPlugin::GetNumAchievements()
{
	if (!m_StatsInitialized)
		return 0;
	return SteamUserStats()->GetNumAchievements();
}

const char *SteamPlugin::GetAchievementId(int index)
{
	if (!m_StatsInitialized)
		return 0;
	return SteamUserStats()->GetAchievementName(index);
}

bool SteamPlugin::SetStat(const char *id, int value)
{
	if (!m_StatsInitialized)
		return false;
	
	if (!SteamUserStats()->SetStat(id, value))
		return false;
	return SteamUserStats()->StoreStats();
}

bool SteamPlugin::SetStat(const char *id, float value)
{
	if (!m_StatsInitialized)
		return false;
	
	if (!SteamUserStats()->SetStat(id, value))
		return false;
	return SteamUserStats()->StoreStats();
}

double SteamPlugin::GetStat(const char *id)
{
	if (!m_StatsInitialized)
		return 0;
	
	float value;
	if (!SteamUserStats()->GetStat(id, &value))
		return 0;
	else
		return (double)value;
}

bool SteamPlugin::ResetAllStats(bool achievements)
{
	if (!m_StatsInitialized)
		return false;
	
	return SteamUserStats()->ResetAllStats(achievements);
}
