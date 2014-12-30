----------------------------------------------------------------------------
-- Nunzio Raciti for Goat <raciti.nunzio@gmail.com>
-- This program is PUBLIC DOMAIN
----------------------------------------------------------------------------

local PLAT_GTK     			= props['PLAT_GTK']
local GoatDir              		= props['GoatDir'] 
local GoatDefaultHome	= props['GoatDefaultHome']
local FileDir  					= props['FileDir']
local FileNameExt 			= props['FileNameExt']
local make_command		= props['make.command']

boards			=	""
field				=	""
board 			= {}
key				= {}
value		    = {}

function parse_boards(filename)
     
	local f = assert(io.open(filename, "r"))
	local lnum = 0

	while true do
		local line = f:read()
		-- exit on eof
		if line == nil then 	break end
		-- search for the board name 
		-- eg. **atmega328**.name=Arduino Duemilanove w/ ATmega328
		s,e=string.find(line, "^%s*(%S-)%.")
		if s~= nil and e ~=nil then
			board[lnum] = string.sub(line,1,e-1)
			-- search for the property
			-- atmega328.**name**=Arduino Duemilanove w/ ATmega328
			s,e=string.find(line, "(%S+)%s*=",e)
			if s~= nil and e ~=nil then
				key[lnum] = string.sub(line,s+1,e-1)
			end
			-- search for the value
			-- atmega328.name=**Arduino Duemilanove w/ ATmega328**
			s,e=string.find(line, "%s*(.+)%s*$",e)
			if s~= nil and e ~=nil then
				value[lnum] = string.sub(line,s+1,e)
			end
			lnum = lnum + 1
		end
	end
	f:close()
end

function find_value (b, k)
	for i,v in ipairs(board) do 
		if v == b and key[i] == k then
			return value[i] 
		end
	end
end

function get_libraries()
	
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

	local FileName 				= props['FileName']
	local AVR_TOOLS_PATH 	= props['arduino.avr_tools_path'] 
	local ARDUINO_DIR			= props['arduino.dir'] 
	local BOARD_TAG          	= props['arduino.board_tag'] 
	local ARDUINO_PORT		= props['arduino.port'] 

	if PLAT_GTK == "1" then
		local f = assert(io.open(FileDir .. "/makefile", "w"))
		f:write("BOARD_TAG          = " .. BOARD_TAG .. "\n")
		f:write("ARDUINO_PORT    = " .. ARDUINO_PORT .. "\n")
		f:write("ARDUINO_LIBS     = ".. get_libraries() .. "\n")
		f:write("TARGET     			= ".. FileName .. "\n\n")
		f:write("PLAT_GTK=1\n")
		f:write("ARDUINO_DIR       = " .. ARDUINO_DIR .. "\n")
		f:write("AVR_TOOLS_PATH = " .. AVR_TOOLS_PATH .. "\n")
		f:write("AVRDUDE        	   = " .. ARDUINO_DIR .. "/hardware/tools/avrdude\n")
		f:write("AVRDUDE_CONF   = " .. ARDUINO_DIR .. "/hardware/tools/avrdude.conf\n")	
		f:write("PARSE_BOARD      =  " .. GoatDir .. "bin/goatlua " .. GoatDir .. "properties/wiring/arduino-mk/ard-parse-boards.lua\n") 
		f:write("include " .. GoatDir .. "properties/wiring/arduino-mk/Arduino.mk\n")
		print(FileDir .. "/makefile")
		f:close()
	else
		local f = assert(io.open(FileDir .. "\\do.bat", "w"))
		f:write("@echo off\n")
		f:write("set GOAT_DEFAULT_HOME=" ..  GoatDefaultHome .."\n")
		f:write("set ARDUINO_COMPORT=" .. ARDUINO_PORT .. "\n")
		f:write("set ARDUINO_LIBS=" .. get_libraries() .. "\n")
		f:write("set ARDUINO_PATH=" .. ARDUINO_DIR .. "\n")

		parse_boards(ARDUINO_DIR .. "\\hardware\\arduino\\boards.txt");
		
		f:write("set ARDUINO_MCU=" .. find_value(BOARD_TAG,"build.mcu") .. "\n")
		f:write("set ARDUINO_PROGRAMMER="..find_value(BOARD_TAG,"upload.protocol") .. "\n")
		f:write("set ARDUINO_VARIANTS=".. find_value(BOARD_TAG,"build.variant") .. "\n")
		f:write("set ARDUINO_CORE=".. find_value(BOARD_TAG,"build.core") .. "\n")
		f:write("set ARDUINO_FCPU=" .. find_value(BOARD_TAG,"build.f_cpu") .. "\n")
		f:write("set ARDUINO_BURNRATE=" .. find_value(BOARD_TAG,"upload.speed") .. "\n")
		f:write("call \"" .. GoatDefaultHome .. "\\wiring\\windows-cli\\abuild.bat\" %1 %2 %3 %4 \n")
		print(FileDir .. "\\do.bat")
		f:close()
	end

	print("... created!")

end

write_makefile()


