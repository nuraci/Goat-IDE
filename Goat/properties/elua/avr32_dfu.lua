------------------------------------------------------------------------
-- Nunzio Raciti for Goat <raciti.nunzio@gmail.com>
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

trace ("start\n")

function onOrOff()
	if editor.SelectionStart == editor.SelectionEnd then
		scite.StripShow("")
		trace ("no\n")
	else
		trace ("yes\n")
		scite.StripShow("!'Explanation:'{}(&Search)\n'Name:'[Name](OK)(Cancel)")
		scite.StripSet(4,"A longer name")
		scite.StripSetList(1,"Apple\nBanana\nOrange\nPear")
	end
end
 
 function onStrip(control,change)
	local changeNames = {'unknow','clicked','change','focusIn','focusOut'}
	if control == 2 and change == 1 then
		local search = scite.StripValue(1)
		print('Search for' .. search)
	else
		print ('OnStrip' .. control ..' ' .. changeName[change+1])
	end
end

onOrOff()

--[[
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
--]]
-- end of script
