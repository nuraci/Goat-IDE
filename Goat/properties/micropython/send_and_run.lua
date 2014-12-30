------------------------------------------------------------------------
-- Nunzio Raciti for Goat <nunzio.raciti@gmail.com>
-- This program is PUBLIC DOMAIN
------------------------------------------------------------------------

local file = props['FilePath'] 
local name = props['FileName'] 
local ext = props['FileExt'] 
local err = ""

goat.ChoiceConsole(GOA_CON_HOST)

if name ==  "" then
	err= "- attempting to send an empty file!"
elseif ext ~= "py"  then
	err = " \"." .. ext .. "\" files are not supported!"
elseif err == "" then 
	goat.ChoiceConsole(GOA_CON_TARGET)
	serial.RawReplySendFile(file)
else	
	output:AddText("Error sending: ".. file .. " " .. err .."\n")
end

-- end of script
