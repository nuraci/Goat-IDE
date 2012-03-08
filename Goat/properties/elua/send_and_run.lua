------------------------------------------------------------------------
-- Nuccio Raciti for Goat <nuccio.raciti@gmail.com>
-- This program is PUBLIC DOMAIN
------------------------------------------------------------------------

local file = props['FilePath'] 
local name = props['FileName'] 
local ext = props['FileExt'] 
local err = ""

goat.ChoiceConsole(GOA_CON_HOST)

if name ==  "" then
	err= "- attempting to send an empty file!"
elseif ext ~= "lua"  then
	err = " \"." .. ext .. "\" files are not supported!"
elseif serial.SendString("recv\n") < 0 then
	err = "accessing the serial port!"
end

if err == "" then 
	goat.ChoiceConsole(GOA_CON_TARGET)
	serial.SendFile(file)
else	
	output:AddText("Error sending: ".. file .. " " .. err .."\n")
end

-- end of script
