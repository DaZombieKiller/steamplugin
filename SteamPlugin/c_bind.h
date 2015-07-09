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

#ifndef STEAMPLUGIN_C_BIND_H_
#define STEAMPLUGIN_C_BIND_H_

extern "C" __declspec(dllexport) void SteamPlugin_Init();
extern "C" __declspec(dllexport) void SteamPlugin_Shutdown();
extern "C" __declspec(dllexport) void SteamPlugin_Update();
extern "C" __declspec(dllexport) double SteamPlugin_CallMethod(const char *method, ...);
extern "C" __declspec(dllexport) int SteamPlugin_GetProperty(const char *property);
extern "C" __declspec(dllexport) bool SteamPlugin_SetProperty(const char *property, double value);
extern "C" __declspec(dllexport) const char *SteamPlugin_GetAchievementId(int index);

#ifdef DARKBASIC_PRO
#include <Windows.h>

__declspec(dllexport) void Constructor(void);
__declspec(dllexport) void Destructor(void);
__declspec(dllexport) void ReceiveCoreDataPtr(LPVOID pCore);
extern "C" __declspec(dllexport) DWORD DBPPlugin_GetAchievementId(DWORD pOldString, int index);
#endif

#endif // STEAMPLUGIN_C_BIND_H_
