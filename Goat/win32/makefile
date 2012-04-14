# Make file for Goat on Linux or compatible OS
# Copyright 2012 by Nuccio Raciti <raciti.nuccio<at>.gmail.com>
#
# Many thanks to:http://www.scintilla.org/ScintillaHistory.html
# The License.txt file describes the conditions under which this software may be distributed.
# This makefile assumes the mingw32 version of GCC 4.x is used and changes will
# be needed to use other compilers.

.SUFFIXES: .cxx .properties

CROSSWIN32 = 1
#DEBUG=1

ifdef CYGWIN
CC = gcc
CPP = g++
DEL = del /q
COPY = copy
WINDRES = windres
endif

ifdef MSYSTEM
DEL = rm
COPY = cp
CC = gcc
CPP = g++
WINDRES = windres
SCINTILLA_PATH = ../../scintilla
endif

ifdef CROSSWIN32
CROSSWIN32PREFIX=i586-mingw32msvc-
DEL = rm
COPY = cp
CC = $(CROSSWIN32PREFIX)gcc
CPP = $(CROSSWIN32PREFIX)g++
WINDRES = $(CROSSWIN32PREFIX)windres
CROS_LIB_PATH = -L/usr/i586-mingw32msvc/lib
SCINTILLA_PATH=/opt/devel/mizar/eclipse_ws/scintilla
endif

GOAT    = Goat.exe
LUAPRG  = goatlua 
LUALIB  = libgoatlua.dll

PROG 	= ../bin/$(GOAT)
LUA_L  = ../bin/$(LUALIB)
LUA_P  = ../bin/$(LUAPRG)

all: $(LUA_L) $(PROG) $(LUA_P)

WIDEFLAGS=-DUNICODE -D_UNICODE

vpath %.h ../src ../../scintilla/include ../../scintilla/win32
vpath %.cxx ../src
vpath %.o ../../scintilla/win32
vpath %.a ../../scintilla/win32

LUA_CORE_OBJS = lapi.o  liolib.o lcode.o ldebug.o ldo.o ldump.o lfunc.o lgc.o llex.o \
		lmem.o lobject.o lopcodes.o lparser.o lstate.o lstring.o \
		ltable.o ltm.o lundump.o lvm.o lzio.o

LUA_LIB_OBJS =	lauxlib.o lbaselib.o ldblib.o lmathlib.o ltablib.o \
		lstrlib.o loadlib.o loslib.o linit.o

LUA_OBJS = $(LUA_CORE_OBJS) $(LUA_LIB_OBJS)

vpath %.c ../lua/src ../lua/src/lib

LUA_INCLUDES = -I ../lua/include
LUA_DEFINES = -DLUA_USER_H=\"scite_lua_win.h\"

INCLUDEDIRS=-I ../../scintilla/include -I ../../scintilla/win32 -I ../src $(LUA_INCLUDES)
RCINCLUDEDIRS=--include-dir ../../scintilla/win32 --include-dir ../src

CBASEFLAGS = -W -Wall $(INCLUDEDIRS) $(WIDEFLAGS)

ifdef DEBUG
CFLAGS=-DDEBUG -g $(CBASEFLAGS)
else
CFLAGS=-DNDEBUG -Os $(CBASEFLAGS)
STRIPFLAG="-Wl,-s"
endif

CXXFLAGS = $(CFLAGS) -Wno-char-subscripts -pedantic -fno-rtti

LDFLAGS=-mwindows -lmsimg32 -lcomctl32 -limm32 -lole32 -luuid -luxtheme $(CROS_LIB_PATH) 

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@


lua.o: ../src/lua.c
	$(CC) $(CFLAGS) -c $< -o $@

.cxx.o:
	$(CPP) $(CXXFLAGS) -c $< -o $@

OBJS =  FileWorker.o Cookie.o Credits.o FilePath.o SciTEBuffers.o SciTEIO.o Exporters.o PropSetFile.o StringHelpers.o \
	StringList.o SciTEProps.o SciTEWin.o SciTEWinBar.o SciTEWinDlg.o MultiplexExtension.o \
	StyleWriter.o UniqueInstance.o Utf8_16.o GoatRes.o GUIWin.o Uart.o Term.o \
	IFaceTable.o DirectorExtension.o JobQueue.o WinMutex.o LuaExtension.o 

DLLS=../bin/Scintilla.dll ../bin/SciLexer.dll

LEXPROPS=../properties/cpp.properties ../properties/elua.properties ../properties/others.properties  
PROPS=../properties/GoatGlobal.properties  $(LEXPROPS)

#ALL:	$(LUA) $(PROG) $(DLLS) $(LUA_SCRIPTS)

clean:
	$(DEL) *.exe *.o *.obj *.dll *.res *.map $(PROG) $(LUA) $(LUA_L) $(LUA_P)

deps:
	$(CPP) -MM $(CXXFLAGS) *.cxx ../src/*.cxx ../lua/src/*.c ../lua/src/lib/*.c >deps.mak

../bin/Scintilla.dll:	$(SCINTILLA_PATH)/bin/Scintilla.dll
	$(COPY) $(SCINTILLA_PATH)/bin/Scintilla.dll $@

../bin/SciLexer.dll:	$(SCINTILLA_PATH)/bin/SciLexer.dll
	$(COPY) $(SCINTILLA_PATH)/bin/SciLexer.dll $@

$(LUA_P): lua.o
	$(CC) $(STRIPFLAG) -Xlinker --subsystem -Xlinker windows -Wl,-rpath,'$$ORIGIN' -L ../bin -lgoatlua \
			-o $@ lua.o -lm

$(PROG):  SciTEBase.o $(OBJS) 
	$(CPP) $(STRIPFLAG) -Xlinker --subsystem -Xlinker windows -Wl,--rpath,'$$ORIGIN' -L ../bin -lgoatlua  -o  $@ $^  $(LDFLAGS) 

$(LUA_L): $(LUA_OBJS) 
	$(CC)  --shared -Wl,--as-needed -Wl,--version-script lua.vers -DWIN32 -Xlinker --subsystem -Xlinker windows  -o $@  $^

# Automatically generate dependencies for most files with "make deps"
include deps.mak

GoatRes.o:	GoatRes.rc Goat.h Goat.exe.manifest
	$(WINDRES) $(RCINCLUDEDIRS) GoatRes.rc $@

# Make sure SciTEBase gets rebuilt (so its about box gets a new
# date stamp) when any of the other objects are updated.
SciTEBase.o: $(OBJS)
