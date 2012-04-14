----------------------------------------------------------------------------
-- Nuccio Raciti for Goat <nuccio.raciti@gmail.com>
-- This program is PUBLIC DOMAIN
----------------------------------------------------------------------------

local make_command	= props['make.command']
local FileDir  				= props['FileDir']

function get_libraries()
	local FileNameExt 	= props['FileNameExt']
	local f = assert(io.open(FileDir .. "/".. FileNameExt, "r"))
	local ret = ""
  
    while true do
		local line = f:read()
		if line == nil then break end
		if string.find(line, "#include")  then
			s,e=string.find(line, "<.*\.h>")
			if s~= nil and e ~=nil then
				ret = ret .." " .. string.sub(line,s+1,e-3)
			end
		end
	end
	f:close()
	return ret
end

function write_makefile() 
	local PLAT_GTK     		 	= props['PLAT_GTK']
	local GoatDir                	= props['GoatDir'] 
	local AVR_TOOLS_PATH 	= props['arduino.avr_tools_path'] 
	local ARDUINO_DIR		= props['arduino.dir'] 
	local BOARD_TAG          	= props['arduino.board_tag'] 
	local ARDUINO_PORT    	= props['arduino.port'] 

	local f = assert(io.open(FileDir .. "/makefile", "w"))
	
	f:write("ARDUINO_DIR       = " .. ARDUINO_DIR .. "\n")
	f:write("BOARD_TAG          = " .. BOARD_TAG .. "\n")
	f:write("ARDUINO_PORT    = " .. ARDUINO_PORT .. "\n\n")
	
	if PLAT_GTK == "1" then
		f:write("PLAT_GTK=1\n")
		f:write("AVR_TOOLS_PATH = " .. AVR_TOOLS_PATH .. "\n")
		f:write("AVRDUDE        	   = " .. ARDUINO_DIR .. "/hardware/tools/avrdude\n")
		f:write("AVRDUDE_CONF   = " .. ARDUINO_DIR .. "/hardware/tools/avrdude.conf\n")	
	else
		local ARDUINO_DOS_DIR = ARDUINO_DIR:gsub("/","\\")
		GoatDir = GoatDir:gsub("\\","/")
		f:write("AVRDUDE        	   = " .. ARDUINO_DIR .. "/hardware/tools/avr/bin/avrdude\n")
		f:write("AVRDUDE_CONF   = " .. ARDUINO_DIR .. "/hardware/tools/avr/etc/avrdude.conf\n")	
		f:write("PATH:=" .. ARDUINO_DOS_DIR .. "\\hardware\\tools\\avr\\utils\\bin\\;$(PATH)\n")
	end

	f:write("ARDUINO_LIBS     =".. get_libraries() .. "\n")
	
	f:write("PARSE_BOARD      =  " .. GoatDir .. "bin/goatlua " .. GoatDir .. "properties/wiring/arduino-mk/ard-parse-boards.lua\n") 
	
	f:write("include " .. GoatDir .. "properties/wiring/arduino-mk/Arduino.mk\n")

	f:close()

	print(FileDir .. "/makefile")
	print("... created!")
end

write_makefile()


