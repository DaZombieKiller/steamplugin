--[[

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

--]]

local ffi = require("ffi")
local SteamPlugin = {}

local sp = ffi.load("SteamPlugin")

ffi.cdef([[

void SteamPlugin_Init();
void SteamPlugin_Shutdown();
void SteamPlugin_Update();
double SteamPlugin_CallMethod(const char *method, ...);
int SteamPlugin_GetProperty(const char *property);
bool SteamPlugin_SetProperty(const char *property, double value);
const char *SteamPlugin_GetAchievementId(int index);

]])

function SteamPlugin:Init()
	sp.SteamPlugin_Init()
end

function SteamPlugin:Shutdown()
	sp.SteamPlugin_Shutdown()
end

function SteamPlugin:Update()
	sp.SteamPlugin_Update()
end

function SteamPlugin:CallMethod(method, ...)
	return sp.SteamPlugin_CallMethod(method, ...)
end

function SteamPlugin:GetProperty(property)
	return sp.SteamPlugin_GetProperty(property)
end

function SteamPlugin:SetProperty(property, value)
	return sp.SteamPlugin_SetProperty(property, value)
end

function SteamPlugin:GetAchievementId(index)
	local str = sp.SteamPlugin_GetAchievementId(index)
	if (str ~= nil) then
		return ffi.string(str)
	else
		return str
	end
end

return SteamPlugin