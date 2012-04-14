#
####
#
CC=gcc
LD=gcc
PREFIX?=../..
INSTALL_BIN= $(PREFIX)/bin
LIBRARY=luausb
LUALIB= -L $(PREFIX)/bin -lgoatlua 

ifeq ($(OS),Windows_NT)
DLLEXT=dll
LDLIBS=-L lib/ -lusb-1.0
CFLAGS=-fvisibility=hidden -I$(PREFIX)/lua/include -Dluaopen_module=luaopen_luausb
else
LDLIBS=-lusb-1.0
DLLEXT=so
CFLAGS=-fvisibility=hidden -fPIC -I$(PREFIX)/lua/include -Dluaopen_module=luaopen_luausb
endif

build:$(LIBRARY).$(DLLEXT)

clean:
	rm -f usb.$(DLLEXT) *.o *.so

cleandist:clean
	rm -f enums.c enums.h structs.c structs.h

install:build
	install -d $(INSTALL_BIN)
	install *.$(DLLEXT) $(INSTALL_BIN)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBRARY).$(DLLEXT): enums.o structs.o compat.o usb.o 
	$(LD) $(CFLAGS) -shared  $^ $(LOADLIBES) -Wl,--rpath .  $(LDLIBS) $(LUALIB) -o $@


usb.c:enums.h structs.h

structs.c enums.c enums.h structs.h:gen.lua
	lua gen.lua

.PHONY:build clean cleandist pureinstall install

# Copyright (c) 2010 Jérôme Vuarand
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
