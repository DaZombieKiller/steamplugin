local Steam = require("SteamPlugin")
local ffi = require("ffi")

function love.load()
	Steam:Init()
	while Steam:GetProperty("SteamAvailable") == 0 do Steam:Update() end
	while Steam:GetProperty("StatsAvailable") == 0 do Steam:Update() end
	
	local NumAchievements = Steam:GetProperty("NumAchievements")
	print("\nPrinting achievements . . .\n")
	for i = 0, NumAchievements do
		print(Steam:GetAchievementId(i))
	end
	
	print("\nResetting stats and achievements . . .\n")
	Steam:CallMethod("ResetAllStats", true)
	
	print("\nUnlocking " .. Steam:GetAchievementId(1) .. " . . .\n")
	Steam:CallMethod("SetAchievement", Steam:GetAchievementId(1))
end

function love.update(dt)
	Steam:Update()
end

function love.draw(dt)
end

function love.quit()
	Steam:Shutdown()
end