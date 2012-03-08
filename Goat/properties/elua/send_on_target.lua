------------------------------------------------------------------------
-- Nuccio Raciti for Goat <nuccio.raciti@gmail.com>
--
-- This program is PUBLIC DOMAIN
------------------------------------------------------------------------

local examples = props['ExamplesDir'] 
local targetFs = props['target.extfs'] 
local FileSelected 
local name
local err = ""

if targetFs == "" then
	targetFs="/mmc/"
end

local  function basename (path)
	local i = string.len(path)

	while string.sub(path, i, i) == "/" and i > 0 do
		path = string.sub(path, 1, i - 1)
		i = i - 1
	end
	while i > 0 do
		if string.sub(path, i, i) == "/" then
			break
		end
		i = i - 1
	end
	if i > 0 then
		path = string.sub(path, i + 1, -1)
	end
	if path == "" then
		path = "/"
	end

	return path
end

goat.ChoiceConsole(GOA_CON_HOST)

goat.AskForFile(examples,"All (*.*)|*.*")

FileSelected = props['FileSelected'] 

name = basename(FileSelected)

if FileSelected ==  "" then
	err= "- attempting to send an empty file!"
elseif serial.SendString("recv ".. targetFs .. name .. "\n") < 0 then
	err = "accessing the serial port!"
end

if err == "" then 
	goat.ChoiceConsole(GOA_CON_TARGET)
	serial.SendFile(FileSelected)
else	
	output:AddText("Error sending: ".. FileSelected .. " " .. err .."\n")
end

-- end of script
