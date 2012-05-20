# Make file for Goat on Linux or compatible OS
# Copyright 2012 by Nuccio Raciti <raciti.nuccio<at>.gmail.com>
#
# Many thanks to:http://www.scintilla.org/ScintillaHistory.html
# The License.txt file describes the conditions under which this software may be distributed.
# This makefile assumes GCC 4.x is used and changes will be needed to use other compilers.
# GNU make does not like \r\n line endings so should be saved to CVS in binary form.

.SUFFIXES: .cxx .o .h .a .c
ifdef CLANG
CC = clang
CCOMP = clang
else
CC = g++
CCOMP = gcc
endif
AR = ar
CAT=cat
SED=sed

ifdef GTK3
GTKVERSION=gtk+-3.0
else
GTKVERSION=gtk+-2.0
endif

#DEBUG=1

# For the Gnome desktop stuff to work, prefix must point to where Gnome thinks it is.
CONFIGFLAGS:=$(shell pkg-config --cflags $(GTKVERSION))
CONFIGLIB:=$(shell pkg-config --libs $(GTKVERSION) gthread-2.0 gmodule-no-export-2.0)
CONFIGTHREADS:=
gnomeprefix:=$(shell pkg-config --variable=prefix $(GTKVERSION) 2>/dev/null)
ifndef prefix
ifdef gnomeprefix
  prefix=$(gnomeprefix)
else
  prefix=/usr
endif
endif
datadir=$(prefix)/share
pixmapdir=$(datadir)/pixmaps
APPDIR=$(prefix)/share/goat

bindir=$(APPDIR)/bin
docsdir=$(APPDIR)/docs
propsdir=$(APPDIR)/properties
examplesdir=$(APPDIR)/examples
pluginsdir=$(APPDIR)/plugins

INSTALL=install

LUAVER=5.1

GOAT     = Goat 
LUAPRG = goatlua 
LUALIB   = libgoatlua.so

PROG 	= ../bin/$(GOAT)
LUA_L  = ../bin/$(LUALIB)
LUA_P  = ../bin/$(LUAPRG)

all: $(LUA_L) $(PROG) $(LUA_P)

vpath %.h ../src ../../scintilla/include
vpath %.cxx ../src

INCLUDEDIRS=-I ../../scintilla/include -I ../src
ifdef CHECK_DEPRECATED
DEPRECATED=-DGDK_PIXBUF_DISABLE_DEPRECATED -DGDK_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED
endif
CXXBASEFLAGS=-W -Wall -Wno-char-subscripts -pedantic -DGTK -DSCI_LEXER $(INCLUDEDIRS) $(DEPRECATED) -fPIC

ifdef DEBUG
CXXTFLAGS=-DDEBUG -g $(CXXFLAGS) $(CXXBASEFLAGS)
else
CXXTFLAGS=-DNDEBUG -Os $(CXXFLAGS) $(CXXBASEFLAGS)
endif

LUA_CORE_OBJS = lapi.o lcode.o ldebug.o ldo.o ldump.o lfunc.o lgc.o llex.o \
		lmem.o lobject.o lopcodes.o lparser.o lstate.o lstring.o \
		ltable.o ltm.o lundump.o lvm.o lzio.o

LUA_LIB_OBJS =	lauxlib.o lbaselib.o ldblib.o liolib.o lmathlib.o ltablib.o \
		lstrlib.o loadlib.o loslib.o linit.o

LUA_OBJS = $(LUA_CORE_OBJS) $(LUA_LIB_OBJS)

vpath %.c ../lua/src ../lua/src/lib

INCLUDEDIRS=-I ../../scintilla/include -I ../src -I../lua/include

.c.o:
	$(CCOMP) $(CFLAGS) $(INCLUDEDIRS) -DLUA_USE_POSIX $(CXXTFLAGS) -c $< -o $@

CXXTFLAGS:=$(CXXTFLAGS) -Wno-long-long

UNAME:=$(shell uname -s)
ifeq ($(UNAME), Linux)
LIBDL:=-ldl
endif
ifeq ($(UNAME), Darwin)
CXXTFLAGS:=$(CXXTFLAGS) -Dunix
endif

.cxx.o:
	$(CC) $(CONFIGFLAGS) $(CXXTFLAGS) -c $< -o $@

clean:
	rm -f *.o *.tmp $(PROG) $(LUA) $(LUA_L) $(LUA_P)

deps:
	$(CC) -MM $(CONFIGFLAGS) $(CXXTFLAGS) *.cxx ../src/*.cxx | sed -e 's/\/usr.* //' | grep [a-zA-Z] >deps.mak

# make should be run in ../../scintilla/gtk to compile all the lexers.
COMPLIB=../../scintilla/bin/scintilla.a 


$(LUA_P): lua.o
	$(CCOMP) $(CFLAGS) $(INCLUDEDIRS) -DLUA_USE_POSIX $(CXXTFLAGS) -c ../src/lua.c -o lua.o
	$(CCOMP) `$(CONFIGTHREADS)` -rdynamic -Wl,--as-needed -Wl,--version-script lua.vers -DGTK $^ -Wl,-rpath,'$$ORIGIN' -L ../bin -lgoatlua \
			-o $@ $(LIBDL) -lm

$(PROG): SciTEGTK.o GUIGTK.o Widget.o \
FilePath.o SciTEBase.o FileWorker.o Cookie.o Credits.o SciTEBuffers.o SciTEIO.o StringList.o Exporters.o StringHelpers.o Uart.o Term.o\
PropSetFile.o MultiplexExtension.o DirectorExtension.o SciTEProps.o StyleWriter.o Utf8_16.o \
	JobQueue.o GTKMutex.o IFaceTable.o  $(COMPLIB) LuaExtension.o 
	$(CC) `$(CONFIGTHREADS)` -rdynamic -Wl,--as-needed -Wl,--version-script lua.vers -DGTK $^ -Wl,-rpath,'$$ORIGIN' -L ../bin -lgoatlua \
			-o $@ $(CONFIGLIB) $(LIBDL) -lstdc++

$(LUA_L): $(LUA_OBJS)
	$(CCOMP)  --shared -fPIC -Wl,--as-needed -Wl,--version-script lua.vers -DGTK $^  -Wl,-soname,$(LUALIB) -o $@
	
# Automatically generate header dependencies with "make deps"
include deps.mak

# The two last install commands will fail if Gnome is not installed or is not at $(prefix).
# This just means no Goat in the Gnome Applications menu
install:
	$(INSTALL) -m 755 -d $(bindir)   $(docsdir) 
	$(INSTALL) -m 777 -d $(propsdir) $(examplesdir)  $(pluginsdir)

	#  Executables
	$(INSTALL) -m 755 $(PROG)  $(bindir)
	$(INSTALL) -m 755 $(LUA_L)  $(bindir)
	$(INSTALL) -m 755 $(LUA_P)  $(bindir)

	for files in ../bin/*.so;  \
	do \
		$(INSTALL) -m 755 $$files $(bindir)/; \
	done

	# Plugins
	$(INSTALL) -m 777 -d $(pluginsdir)/libusb
	for files in ../plugins/libusb/*;  \
	do \
		$(INSTALL) -m 777 $$files $(pluginsdir)/libusb/; \
	done

	# Properties
	$(INSTALL) -m 777 ../properties/others.properties $(propsdir);
	$(INSTALL) -m 777 ../properties/wiring.properties $(propsdir);
	$(INSTALL) -m 777 ../properties/elua.properties $(propsdir);
	$(INSTALL) -m 777 ../properties/Goat.properties $(propsdir); 
	$(INSTALL) -m 777 ../properties/GoatGlobal.properties $(propsdir);
	$(INSTALL) -m 777 ../properties/locale.it.properties $(propsdir);
	$(INSTALL) -m 777 ../properties/cpp.properties $(propsdir);

	# Properties (eLua)
	$(INSTALL) -m 777 -d $(propsdir)/elua
	for files in ../properties/elua/*;  \
	do \
		$(INSTALL) -m 644 $$files $(propsdir)/elua; \
	done
	
	# Properties (Wiring)
	$(INSTALL) -m 777 -d $(propsdir)/wiring
	for files in ../properties/wiring/*.api;  \
	do \
		$(INSTALL) -m 644 $$files $(propsdir)/wiring; \
	done
	$(INSTALL) -m 777 -d $(propsdir)/wiring/arduino-mk
	for files in ../properties/wiring/arduino-mk/*;  \
	do \
		$(INSTALL) -m 644 $$files $(propsdir)/wiring/arduino-mk; \
	done
	$(INSTALL) -m 777 -d $(propsdir)/wiring/windows-cli
	for files in ../properties/wiring/windows-cli/*;  \
	do \
		$(INSTALL) -m 644 $$files $(propsdir)/wiring/windows-cli; \
	done
	
	# Docs
	$(INSTALL) -m 777 -d $(docsdir)/img $(docsdir)/it $(docsdir)/en  
	for files in ../docs/img/*;  \
	do \
		$(INSTALL) -m 644 $$files $(docsdir)/img; \
	done
	for files in ../docs/en/*;  \
	do \
		$(INSTALL) -m 644 $$files $(docsdir)/en; \
	done
	for files in ../docs/it/*;  \
	do \
		$(INSTALL) -m 644 $$files $(docsdir)/it; \
	done

	# Examples
	$(INSTALL) -m 777 -d $(examplesdir)/eLua
	$(INSTALL) -m 777 ../examples/eLua/README $(examplesdir)/eLua; 
	$(INSTALL) -m 777 -d $(examplesdir)/eLua/hangman
	$(INSTALL) -m 777 ../examples/eLua/hangman/*.lua $(examplesdir)/eLua/hangman; 
	$(INSTALL) -m 777 -d $(examplesdir)/eLua/hanoi
	$(INSTALL) -m 777 ../examples/eLua/hanoi/*.lua $(examplesdir)/eLua/hanoi; 
	$(INSTALL) -m 777 -d $(examplesdir)/eLua/hello
	$(INSTALL) -m 777 ../examples/eLua/hello/*.lua $(examplesdir)/eLua/hello; 
	$(INSTALL) -m 777 -d $(examplesdir)/eLua/http-led
	$(INSTALL) -m 777 ../examples/eLua/http-led/*.lua $(examplesdir)/eLua/http-led; 
	$(INSTALL) -m 777 ../examples/eLua/http-led/*.pht $(examplesdir)/eLua/http-led; 
	$(INSTALL) -m 777 -d $(examplesdir)/eLua/info
	$(INSTALL) -m 777 ../examples/eLua/info/*.lua $(examplesdir)/eLua/info; 
	$(INSTALL) -m 777 -d $(examplesdir)/eLua/led
	$(INSTALL) -m 777 ../examples/eLua/led/*.lua $(examplesdir)/eLua/led; 
	$(INSTALL) -m 777 -d $(examplesdir)/eLua/rtc
	$(INSTALL) -m 777 ../examples/eLua/rtc/*.lua $(examplesdir)/eLua/rtc; 
	$(INSTALL) -m 777 -d $(examplesdir)/eLua/serial-passthrou
	$(INSTALL) -m 777 ../examples/eLua/serial-passthrou/*.lua $(examplesdir)/eLua/serial-passthrou; 	
	$(INSTALL) -m 777 -d $(examplesdir)/Arduino
	$(INSTALL) -m 777 -d $(examplesdir)/Arduino/Blink
	$(INSTALL) -m 777 ../examples/Arduino/Blink/*.ino $(examplesdir)/Arduino/Blink; 

ifdef gnomeprefix
	$(INSTALL) -m 755 -d $(DESTDIR)$(datadir)/applications $(DESTDIR)$(pixmapdir)
	$(CAT) Goat.desktop | $(SED)  -e "s,%APP%,$(bindir)/$(GOAT)," > Goat.tmp
	$(INSTALL) -m 755 Goat.tmp $(DESTDIR)$(datadir)/applications/Goat.desktop
	$(INSTALL) -m 644 Goat.png $(DESTDIR)$(pixmapdir)/Goat.png
endif

uninstall:
	rm -f $(DESTDIR)$(bindir)/Goat
	rm -rf $(DESTDIR)$(APPDIR)
ifdef gnomeprefix
	rm -f $(DESTDIR)$(datadir)/applications/Goat.desktop
	rm -f $(DESTDIR)$(pixmapdir)/Goat.png
endif
