# Make file for SciTE on Windows
# Copyright 1998-2010 by Neil Hodgson <neilh@scintilla.org>
# The License.txt file describes the conditions under which this software may be distributed.
# This makefile assumes the mingw32 version of GCC 4.x is used and changes will
# be needed to use other compilers.

.SUFFIXES: .cxx .properties

CROSSWIN32 = 1
DEBUG=1

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

PROG	= ../bin/Goat.exe
PROGSTATIC = ../bin/GoatIDE.exe

WIDEFLAGS=-DUNICODE -D_UNICODE

vpath %.h ../src ../../scintilla/include ../../scintilla/win32
vpath %.cxx ../src
vpath %.o ../../scintilla/win32
vpath %.a ../../scintilla/win32

ifndef NO_LUA
LUA_CORE_OBJS = lapi.o lcode.o ldebug.o ldo.o ldump.o lfunc.o lgc.o llex.o \
		lmem.o lobject.o lopcodes.o lparser.o lstate.o lstring.o \
		ltable.o ltm.o lundump.o lvm.o lzio.o

LUA_LIB_OBJS =	lauxlib.o lbaselib.o ldblib.o liolib.o lmathlib.o ltablib.o \
		lstrlib.o loadlib.o loslib.o linit.o

LUA_OBJS = LuaExtension.o $(LUA_CORE_OBJS) $(LUA_LIB_OBJS)

vpath %.c ../lua/src ../lua/src/lib

LUA_INCLUDES = -I ../lua/include
#LUA_SCRIPTS = ..\bin\SciTEExtension.lua ..\bin\SciTEStartup.lua
LUA_DEFINES = -DLUA_USER_H=\"scite_lua_win.h\"
else
LUA_DEFINES = -DNO_LUA
endif

INCLUDEDIRS=-I ../../scintilla/include -I ../../scintilla/win32 -I ../src $(LUA_INCLUDES)
RCINCLUDEDIRS=--include-dir ../../scintilla/win32 --include-dir ../src

CBASEFLAGS = -W -Wall $(INCLUDEDIRS) $(LUA_DEFINES) $(WIDEFLAGS)

ifdef DEBUG
CFLAGS=-DDEBUG -g $(CBASEFLAGS)
else
CFLAGS=-DNDEBUG -Os $(CBASEFLAGS)
STRIPFLAG="-Wl,-s"
endif

CXXFLAGS = $(CFLAGS) -Wno-char-subscripts -pedantic -fno-rtti

LDFLAGS=-mwindows -lmsimg32 -lcomctl32 -limm32 -lole32 -luuid -luxtheme $(CROS_LIB_PATH) $(LUA_LDFLAGS)

.cxx.o:
	$(CPP) $(CXXFLAGS) -c $< -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

OTHER_OBJS	= FileWorker.o Cookie.o Credits.o FilePath.o SciTEBuffers.o SciTEIO.o Exporters.o PropSetFile.o StringHelpers.o \
StringList.o SciTEProps.o SciTEWin.o SciTEWinBar.o SciTEWinDlg.o MultiplexExtension.o \
StyleWriter.o UniqueInstance.o \
Utf8_16.o SciTERes.o GUIWin.o Uart.o Term.o \
IFaceTable.o DirectorExtension.o JobQueue.o WinMutex.o $(LUA_OBJS)

OBJS = SciTEBase.o $(OTHER_OBJS)

DLLS=../bin/Scintilla.dll ../bin/SciLexer.dll

#++Autogenerated -- run scintilla/src/LexGen.py to regenerate
#**LEXPROPS=\\\n\(../bin/\* \)
LEXPROPS=\
../bin/abaqus.properties ../bin/ada.properties ../bin/asm.properties \
../bin/asn1.properties ../bin/au3.properties ../bin/ave.properties \
../bin/baan.properties ../bin/blitzbasic.properties ../bin/bullant.properties \
../bin/caml.properties ../bin/cmake.properties ../bin/cobol.properties \
../bin/conf.properties ../bin/cpp.properties ../bin/csound.properties \
../bin/css.properties ../bin/d.properties ../bin/eiffel.properties \
../bin/erlang.properties ../bin/escript.properties ../bin/flagship.properties \
../bin/forth.properties ../bin/fortran.properties ../bin/freebasic.properties \
../bin/gap.properties ../bin/haskell.properties ../bin/html.properties \
../bin/inno.properties ../bin/kix.properties ../bin/latex.properties \
../bin/lisp.properties ../bin/lot.properties ../bin/lout.properties \
../bin/lua.properties ../bin/matlab.properties ../bin/metapost.properties \
../bin/mmixal.properties ../bin/modula3.properties ../bin/nimrod.properties \
../bin/nncrontab.properties ../bin/nsis.properties ../bin/opal.properties \
../bin/others.properties ../bin/pascal.properties ../bin/perl.properties \
../bin/pov.properties ../bin/powerpro.properties ../bin/powershell.properties \
../bin/ps.properties ../bin/purebasic.properties ../bin/python.properties \
../bin/r.properties ../bin/rebol.properties ../bin/ruby.properties \
../bin/scriptol.properties ../bin/smalltalk.properties \
../bin/sorcins.properties ../bin/specman.properties ../bin/spice.properties \
../bin/sql.properties ../bin/tacl.properties ../bin/tal.properties \
../bin/tcl.properties ../bin/tex.properties ../bin/txt2tags.properties \
../bin/vb.properties ../bin/verilog.properties ../bin/vhdl.properties \
../bin/yaml.properties
#--Autogenerated -- end of automatically generated section

PROPS=../bin/SciTEGlobal.properties ../bin/abbrev.properties $(LEXPROPS)

#ALL:	$(PROG) $(PROGSTATIC) $(DLLS) $(PROPS) $(LUA_SCRIPTS)
ALL:	$(PROG) $(DLLS) $(PROPS) $(LUA_SCRIPTS)

clean:
	$(DEL) *.exe *.o *.obj *.dll *.res *.map

deps:
	$(CPP) -MM $(CXXFLAGS) *.cxx ../src/*.cxx ../lua/src/*.c ../lua/src/lib/*.c >deps.mak

embedded: ../src/Embedded.properties

../bin/Scintilla.dll:	$(SCINTILLA_PATH)/bin/Scintilla.dll
	$(COPY) $(SCINTILLA_PATH)/bin/Scintilla.dll $@

../bin/SciLexer.dll:	$(SCINTILLA_PATH)/bin/SciLexer.dll
	$(COPY) $(SCINTILLA_PATH)/bin/SciLexer.dll $@

# Using '/' rather than '\' as that allows make to match the next rule
../bin/%.properties:	../src/%.properties
	$(COPY) $(subst /,\,$^) $(subst /,\,$@)

# Normally distributed rather than built as may not have grep on all machines
# Copy all non-comment lines from all the properties files into one combined file
../src/Embedded.properties: $(PROPS)
	grep -v -h "^[#]" $(PROPS) >..\src\Embedded.properties

$(PROG): $(OBJS)
	$(CPP) $(STRIPFLAG) -Xlinker --subsystem -Xlinker windows -o  $@ $^ $(LDFLAGS)

OBJSSTATIC = goat_static.o SciTEWinBar.o SciTEWinDlg.o MultiplexExtension.o \
	StyleWriter.o UniqueInstance.o SciTEBase.o FileWorker.o Cookie.o Credits.o FilePath.o SciTEBuffers.o \
	SciTEIO.o Exporters.o PropSetFile.o StringHelpers.o StringList.o SciTEProps.o ScintillaWinL.o \
	ScintillaBaseL.o Editor.o Catalogue.o Accessor.o WordList.o CharacterSet.o \
	LexerModule.o LexerSimple.o LexerBase.o \
	ExternalLexer.o StyleContext.o Lexers.a PropSetSimple.o RunStyles.o \
	ContractionState.o CharClassify.o Decoration.o Document.o CellBuffer.o CallTip.o Sc1Res.o PlatWin.o \
	PositionCache.o UniConversion.o Utf8_16.o GUIWin.o KeyMap.o Indicator.o LineMarker.o RESearch.o Selection.o Style.o \
	ViewStyle.o AutoComplete.o IFaceTable.o DirectorExtension.o MultiplexExtension.o XPM.o PerLine.o Uart.o Term.o\
	JobQueue.o WinMutex.o $(LUA_OBJS)

$(PROGSTATIC): $(OBJSSTATIC)
	$(CPP) $(STRIPFLAG) -Xlinker --subsystem -Xlinker windows -o $@ $^ $(LDFLAGS)

# Automatically generate dependencies for most files with "make deps"
include deps.mak

goat_static.o: SciTEWin.cxx SciTEWin.h Goat.h PropSetFile.h \
 SString.h Scintilla.h Extender.h FilePath.h SciTEBase.h JobQueue.h
	$(CPP) $(CXXFLAGS) -D STATIC_BUILD -c $< -o $@

GoatRes.o:	GoatRes.rc Goat.h Goat.exe.manifest
	$(WINDRES) $(RCINCLUDEDIRS) GoatRes.rc $@

# Also depends on ../src/Embedded.properties but may not want to build everywhere
# so must explicitly ask to build it.
GoatIDERes.o:	GoatRes.rc Goat.h Goat.exe.manifest
	$(WINDRES) $(RCINCLUDEDIRS) GoatRes.rc --define STATIC_BUILD $@

# Make sure SciTEBase gets rebuilt (so its about box gets a new
# date stamp) when any of the other objects are updated.
SciTEBase.o: $(OTHER_OBJS)