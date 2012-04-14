boards_txt 	= '/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/boards.txt'
boards			=	""
field				=	""
board 			= {}
key				= {}
value		    = {}

function usage()  

page = [[
 
ard-parse-boards - Read data from the Arduino boards.txt file\n
\n
   Dump all the data in the file:\n
    $ ard-parse-boards --dump\n

    See which boards we know about:
    $ ard-parse-boards --boards

    Look for a particular board...
    $ ard-parse-boards --find uno

    ...multiple terms are implicitly ANDed:
    $ ard-parse-boards --find duemil 328

    Dump all the data for a particular board:
    $ ard-parse-boards atmega328

    Extract a particular field:
    $ ard-parse-boards atmega328 build.f_cpu
 

The Arduino software package ships with a boards.txt file which tells
the Arduino IDE details about particular hardware. So when the user
says he's got a shiny new Arduino Uno, boards.txt knows that it has a
16MHz ATmega328 on it. It would be nice to access these data from the
command line too.

In normal operation you simply specify the tag given to the board in
the boards.txt file, and optionally a field name. This program then
extracts the data to STDOUT.

Most boards have names which are quite unwieldy, so we always refer to
a board by a tag, not its name. Strictly the tag is the bit before the
first dot in the boards.txt key. You can see a list of board tags and
names with the C<--boards> option.
]]

	print ( page)
end

--[[
	this function searchs inside the boards.txt file:
	atmega328.name=Arduino Duemilanove w/ ATmega328
--]]
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

function GetOptions(a)
	
	if	a[1] == nil then
		return nil
	end

	i, j = string.find(a[1], "--boards_txt=")
	if i ~= nil then
		boards_txt = string.sub(a[1],j+1) 
		if a[2] ~= nil then
			boards	=	a[2]
		end
		if a[3] ~= nil then
			field	=	a[3]
		end
		return boards_txt, boards, field
	else 
		i, j = string.find(a[1], "--boards" )
		if i ~= nil then
			print (a[1] .. " not supported!") 
			return nil
		end
	end
	
	i, j = string.find(a[1], "--find" )
	if i ~= nil then
		print (a[1] .. " not supported!") 
		return nil
	end
	
	i, j = string.find(a[1], "--dump" )
	if i ~= nil then
		print (a[1] .. " not supported!") 
		return nil
	end

	i, j = string.find(a[1], "--help" )
	if i ~= nil then
		usage()
		return nil
	end
	
	i, j = string.find(a[1], "--info" )
	if i ~= nil then
		usage()
		return nil
	end
 
end

function find_value (b, k)
	for i,v in ipairs(board) do 
		if v == b and key[i] == k then
			return value[i] 
		end
	end

end

bs, b, k = GetOptions( arg )

if bs ~= nil then
    parse_boards(bs);
	print (find_value(b,k))
end




