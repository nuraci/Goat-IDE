require 'markdown'

local charset = 'UTF-8'
local file_index = "index.html"

------------------------------------------------------------------------------

function print(...)
	local t = {...}
	for i=1,select('#', ...) do
		t[i] = tostring(t[i])
	end
	io.write(table.concat(t, '\t')..'\n')
end

function header()
	print([[
<?xml version="1.0" encoding="]]..charset..[["?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en"
lang="en">
<head>
<title>luausb @ piratery.net</title>
<meta http-equiv="Content-Type" content="text/html; charset=]]..charset..[["/>
<link rel="stylesheet" href="doc.css" type="text/css"/>
</head>
<body>
<div class="chapter" id="header">
<img width="128" height="128" alt="prtr-luausb" src="logo.png"/>
<p>A low level Lua binding for the libusb1 library.</p>
</div>
]])
end

function footer()
	print([[
<div class="chapter" id="footer">
<small>Last update: ]]..os.date"%Y-%m-%d %H:%M:%S %Z"..[[</small>
</div>
</body>
</html>
]])
end

local chapterid = 0

function chapter(title, text, sections, raw)
	chapterid = chapterid+1
	local text = text:gsub("%%chapterid%%", tostring(chapterid))
	if not raw then
		text = markdown(text)
	end
	if sections then
		for _,section in ipairs(sections) do
			section = section:gsub("%%chapterid%%", tostring(chapterid))
			text = text..[[
<div class="section">
]]..markdown(section)..[[
</div>]]
		end
	end
	print([[
<div class="chapter">
<h1>]]..tostring(chapterid).." - "..title..[[</h1>
]]..text..[[
</div>
]])
end

------------------------------------------------------------------------------

io.output(file_index)

header()

chapter("About", [[
The prtr-luausb module is a simple, low-level binding of the libusb1 API for Lua.

The name luausb is not original, but it reflects the purpose of the library. The prtr- prefix (a contraction for piratery.net, the website domain) is used because other libusb bindings may emerge with the same naming problems.

## Support

All support is done through the [Lua mailing list](http://www.lua.org/lua-l.html). If the traffic becomes too important a specialized mailing list will be created.

Feel free to ask for further developments. I can't guarantee that I'll develop everything you ask, but I want my code to be as useful as possible, so I'll do my best to help you. You can also send me request or bug reports (for code and documentation) directly at [jerome.vuarand@gmail.com](mailto:jerome.vuarand@gmail.com).

## Credits

This module is written and maintained by [Jérôme Vuarand](mailto:jerome.vuarand@gmail.com).

It is available under a [MIT-style license](LICENSE.txt).

]])

chapter('Installation', [[
prtr-luausb sources are available in its [Mercurial repository](http://hg.piratery.net/luausb/):

    hg clone http://hg.piratery.net/luausb/

Tarballs of the latest code can be downloaded directly from there: as [gz](http://hg.piratery.net/luausb/archive/tip.tar.gz), [bz2](http://hg.piratery.net/luausb/archive/tip.tar.bz2) or [zip](http://hg.piratery.net/luausb/archive/tip.zip).

Finally, I published some rockspecs:

    luarocks install prtr-luausb
]])

chapter('Manual', [[
This module is still a work in progress. As such no documentation is maintained. Look at the source for more information.

To use it:

    require 'usb'
]])

footer()

------------------------------------------------------------------------------

--[[
Copyright (c) 2010 Jérôme Vuarand

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
]]

-- vi: ts=4 sts=4 sw=4 noet
