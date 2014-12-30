------------------------------------------------------------------------
-- Nunzio Raciti for Goat <raciti.nunzio@gmail.com>
-- This program is PUBLIC DOMAIN
------------------------------------------------------------------------

local targetFs = props['target.extfs'] 
local FileSelected = props['FileName'] 
local err = ""

if targetFs == "" then
	targetFs="/mmc/"
end

goat.ChoiceConsole(GOA_CON_HOST)

goat.AskQuestion("Enter the file name")

FileSelected = props['Answer'] 

if FileSelected ==  "" then
	err= "- attempting to send an empty file!"
elseif serial.SendString("lua ".. targetFs .. FileSelected .. "\n") < 0 then
	err = "accessing the serial port!"
end

if err == "" then 
	goat.ChoiceConsole(GOA_CON_TARGET)
else	
	output:AddText("Error sending: ".. FileSelected .. " " .. err .."\n")
end


-- end of script
