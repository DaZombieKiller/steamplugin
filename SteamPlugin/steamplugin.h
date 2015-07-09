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

#ifndef STEAMPLUGIN_STEAMPLUGIN_H_
#define STEAMPLUGIN_STEAMPLUGIN_H_

#include <steam_api.h>

class SteamPlugin
{
public:
	SteamPlugin();
	virtual ~SteamPlugin(void);
	
	bool Update();
	double CallMethod(va_list args, const char *method);
	int GetProperty(const char *property);
	bool SetProperty(const char *property, double value);
	const char *GetAchievementId(int index);
	
	STEAM_CALLBACK(SteamPlugin, OnAchievementStored, UserAchievementStored_t, m_CallbackAchievementStored);
	STEAM_CALLBACK(SteamPlugin, OnUserStatsReceived, UserStatsReceived_t,     m_CallbackUserStatsReceived);
	STEAM_CALLBACK(SteamPlugin, OnUserStatsStored,   UserStatsStored_t,       m_CallbackUserStatsStored);

private:
	bool InitSteam();
	bool RequestStats();
	bool ResetAllStats(bool achivements);
	
	bool SetAchievement(const char *id);
	bool IsAchieved(const char *id);
	bool ClearAchievement(const char *id);
	int GetNumAchievements();
	
	bool SetStat(const char *id, int value);
	bool SetStat(const char *id, float value);
	double GetStat(const char *id);
	
	bool m_SteamInitialized;
	bool m_StatsInitialized;
	int m_AppId;
};

#endif // STEAMPLUGIN_STEAMPLUGIN_H_
