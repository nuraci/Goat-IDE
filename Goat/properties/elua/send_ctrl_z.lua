------------------------------------------------------------------------
-- Nuccio Raciti for Goat <nuccio.raciti@gmail.com>
--
-- This program is PUBLIC DOMAIN
------------------------------------------------------------------------

local ctrl_z ="\026"
local err = ""

goat.ChoiceConsole(GOA_CON_HOST)

if serial.SendString(ctrl_z) < 0 then
	err = "accessing the serial port!"
end

if err == "" then 
	goat.ChoiceConsole(GOA_CON_TARGET)
else	
	output:AddText("Error sending <Ctrl+Z> , " .. err .."\n")
end

-- end of script
