# Makefile for Lua BitOp -- a bit operations library for Lua 5.1.
# To compile with MSVC please run: msvcbuild.bat
# To compile with MinGW please run: mingw32-make -f Makefile.mingw

# Include path where lua.h, luaconf.h and lauxlib.h reside:
PREFIX?=../..
INCLUDES=-I$(PREFIX)/lua/include

DEFINES=
# Use this if you need to compile for an old ARM ABI with swapped FPA doubles
#DEFINES= -DSWAPPED_DOUBLE

# Lua executable name. Used to find the install path and for testing.
LUA= lua

CC= gcc
SOCFLAGS= -fPIC
SOCC= $(CC) -shared $(SOCFLAGS)
CFLAGS= -Wall -O2 -fomit-frame-pointer $(SOCFLAGS) $(DEFINES) $(INCLUDES)
RM= rm -f
INSTALL= install -p
INSTALL_BIN=$(PREFIX)/bin

ifeq ($(OS),Windows_NT)
DLLEXT=dll
else
DLLEXT=so
endif

MODNAME= bit
MODSO= $(MODNAME).$(DLLEXT)

all: $(MODSO)

# Alternative target for compiling on Mac OS X:
macosx:
	$(MAKE) all "SOCC=MACOSX_DEPLOYMENT_TARGET=10.3 $(CC) -dynamiclib -single_module -undefined dynamic_lookup $(SOCFLAGS)"

$(MODSO): $(MODNAME).o
	$(SOCC)  -o $@ $<
	strip --strip-unneeded $@

install: $(MODSO)
	$(INSTALL) $(MODSO) $(INSTALL_BIN)

test: $(MODSO)
	@$(LUA) bittest.lua && echo "basic test OK"
	@$(LUA) nsievebits.lua && echo "nsievebits test OK"
	@$(LUA) md5test.lua && echo "MD5 test OK"

clean:
	$(RM) *.o *$(DLLEXT) *.obj *.lib *.exp *.dll *.manifest

.PHONY: all macosx install test clean

