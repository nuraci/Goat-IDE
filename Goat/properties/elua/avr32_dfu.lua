------------------------------------------------------------------------
-- Nuccio Raciti for Goat <nuccio.raciti@gmail.com>
--
-- This program is PUBLIC DOMAIN
------------------------------------------------------------------------

local examples 	= props['ExamplesDir'] 
local Bindir      		= props['BinDir'] 
local Cpu        		= props['TargetCPU'] 
local DfuExec  		= Bindir .. "/".."dfu-programmer"
local DfuDebug  	= " --debug 40"
local FileSelected = ""
local err 				= ""

 scite.StripShow("!'Explanation:'{}(&Search)\n'Name:'[Name](OK)(Cancel)")

goat.ChoiceConsole(GOA_CON_HOST)

goat.AskForFile(examples,"Flash Image (*.hex)|*.hex")

FileSelected = props['FileSelected'] 

if FileSelected ==  "" then
	err= "- attempting to use an empty file!"
elseif serial.SendString("Close the Serial port first\n") >= 0 then
	err = "Please close the serial port and switch the board in DFU mode!"
end

err="does not work"

if err == "" then 
    output:AddText(DfuExec .. " " .. Cpu .. " erase" .. DfuDebug .. "\n")
	output:AddText(os.execute(DfuExec .. " " .. Cpu .. " erase" .. DfuDebug).."\n")
	output:AddText(DfuExec .. " " .. Cpu .. " flash" .. DfuDebug .. " " .. FileSelected .. "\n")
	output:AddText(os.execute(DfuExec .. " " .. Cpu .. " flash" .. DfuDebug .. " " .. FileSelected).."\n")
	output:AddText(DfuExec .. " " .. Cpu .. " reset" .. DfuDebug .. "\n")
	output:AddText(os.execute(DfuExec .. " " .. Cpu .. " reset" .. DfuDebug).."\n")
	output:AddText(os.execute(DfuExec .. " " .. Cpu .. " reset" .. DfuDebug).."\n")
else	
	output:AddText("Error programming flash: ".. FileSelected .. " " .. err .."\n")
end

-- end of script
