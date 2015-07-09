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

#include "c_bind.h"
#include "steamplugin.h"
#include <stdarg.h>
#ifdef DARKBASIC_PRO
#include "globstruct.h"
#endif

static SteamPlugin *Steam;
#ifdef DARKBASIC_PRO
GlobStruct *g_pGlob = NULL;

__declspec(dllexport) void Constructor(void)
{
	// Create memory here
}

__declspec(dllexport) void Destructor(void)
{
	// Free memory here
}

__declspec(dllexport) void ReceiveCoreDataPtr(LPVOID pCore)
{
	// Get Core Data Pointer here
	g_pGlob = (GlobStruct *)pCore;
}
#endif

extern "C" __declspec(dllexport) void SteamPlugin_Init()
{
	if (!Steam)
		Steam = new SteamPlugin();
}

extern "C" __declspec(dllexport) void SteamPlugin_Shutdown()
{
	if (Steam)
		delete[] Steam;
}

extern "C" __declspec(dllexport) void SteamPlugin_Update()
{
	if (Steam)
		Steam->Update();
}

extern "C" __declspec(dllexport) double SteamPlugin_CallMethod(const char *method, ...)
{
	va_list args;
	va_start(args, method);
	double result = Steam->CallMethod(args, method);
	va_end(args);
	return result;
}

extern "C" __declspec(dllexport) int SteamPlugin_GetProperty(const char *property)
{
	return Steam->GetProperty(property);
}

extern "C" __declspec(dllexport) bool SteamPlugin_SetProperty(const char *property, double value)
{
	return Steam->SetProperty(property, value);
}

extern "C" __declspec(dllexport) const char *SteamPlugin_GetAchievementId(int index)
{
	return Steam->GetAchievementId(index);
}

#ifdef DARKBASIC_PRO
extern "C" __declspec(dllexport) DWORD DBPPlugin_GetAchievementId(DWORD pOldString, int index)
{
	if (pOldString)
		g_pGlob->CreateDeleteString((DWORD *)&pOldString, 0);
	LPSTR pReturnString = NULL;
	const char *achievement = Steam->GetAchievementId(index);

	if (achievement)
	{
		DWORD dwSize = strlen(achievement);
		g_pGlob->CreateDeleteString((DWORD *)&pReturnString, dwSize + 1);
		strcpy(pReturnString, achievement);
	}

	return (DWORD)pReturnString;
}
#endif