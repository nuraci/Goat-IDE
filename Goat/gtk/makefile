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
CONFIGLIB:=$(shell pkg-config --libs $(GTKVERSION) gthread-2.0)
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
docdir=$(APPDIR)/docs
propdir=$(APPDIR)/properties
examplesdir=$(APPDIR)/examples

INSTALL=install

PROG_EXE = Goat 
PROG	 = ../bin/$(PROG_EXE)
DFU-P	 = ../bin/dfu-programmer

all: $(PROG)

vpath %.h ../src ../../scintilla/include
vpath %.cxx ../src

INCLUDEDIRS=-I ../../scintilla/include -I ../src
ifdef CHECK_DEPRECATED
DEPRECATED=-DGDK_PIXBUF_DISABLE_DEPRECATED -DGDK_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED
endif
CXXBASEFLAGS=-W -Wall -Wno-char-subscripts -pedantic -DGTK -DSCI_LEXER $(INCLUDEDIRS) $(DEPRECATED)

ifdef DEBUG
CXXTFLAGS=-DDEBUG -g $(CXXBASEFLAGS)
else
CXXTFLAGS=-DNDEBUG -Os $(CXXBASEFLAGS)
endif

ifndef NO_LUA
LUA_CORE_OBJS = lapi.o lcode.o ldebug.o ldo.o ldump.o lfunc.o lgc.o llex.o \
		lmem.o lobject.o lopcodes.o lparser.o lstate.o lstring.o \
		ltable.o ltm.o lundump.o lvm.o lzio.o

LUA_LIB_OBJS =	lauxlib.o lbaselib.o ldblib.o liolib.o lmathlib.o ltablib.o \
		lstrlib.o loadlib.o loslib.o linit.o

LUA_OBJS = LuaExtension.o $(LUA_CORE_OBJS) $(LUA_LIB_OBJS)

vpath %.c ../lua/src ../lua/src/lib

INCLUDEDIRS=-I ../../scintilla/include -I ../src -I../lua/include

.c.o:
	$(CCOMP) $(CFLAGS) $(INCLUDEDIRS) -DLUA_USE_POSIX $(CXXTFLAGS) -c $< -o $@

else
CXXFLAGS+=-DNO_LUA
endif

CXXFLAGS:=$(CXXFLAGS) $(CXXTFLAGS) -Wno-long-long

UNAME:=$(shell uname -s)
ifeq ($(UNAME), Linux)
LIBDL:=-ldl
endif
ifeq ($(UNAME), Darwin)
CXXFLAGS:=$(CXXFLAGS) -Dunix
endif

.cxx.o:
	$(CC) $(CONFIGFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o $(PROG)

deps:
	$(CC) -MM $(CONFIGFLAGS) $(CXXFLAGS) *.cxx ../src/*.cxx | sed -e 's/\/usr.* //' | grep [a-zA-Z] >deps.mak

# make should be run in ../../scintilla/gtk to compile all the lexers.
COMPLIB=../../scintilla/bin/scintilla.a

$(PROG): SciTEGTK.o GUIGTK.o Widget.o \
FilePath.o SciTEBase.o FileWorker.o Cookie.o Credits.o SciTEBuffers.o SciTEIO.o StringList.o Exporters.o StringHelpers.o Uart.o Term.o\
PropSetFile.o MultiplexExtension.o DirectorExtension.o SciTEProps.o StyleWriter.o Utf8_16.o \
	JobQueue.o GTKMutex.o IFaceTable.o $(COMPLIB) $(LUA_OBJS)
	$(CC) `$(CONFIGTHREADS)` -rdynamic -Wl,--as-needed -Wl,--version-script lua.vers -DGTK $^ -o $@ $(CONFIGLIB) $(LIBDL) -lstdc++

# Automatically generate header dependencies with "make deps"
include deps.mak

# The two last install commands will fail if Gnome is not installed or is not at $(prefix).
# This just means no Goat in the Gnome Applications menu
install:
	$(INSTALL) -m 755 -d $(bindir) $(docdir) 
	$(INSTALL) -m 777 -d $(propdir) $(examplesdir)

	$(INSTALL) -m 755 $(PROG) $(bindir)
	$(INSTALL) -m 755 $(DFU-P) $(bindir)
	
	for files in ../properties/*.*; \
	do \
		$(INSTALL) -m 777 $$files $(propdir); \
	done

	for files in ../docs/*;  \
	do \
		$(INSTALL) -m 644 $$files $(docdir); \
	done

	for files in ../examples/*/*;  \
	do \
		$(INSTALL) -m 777 $$files $(examplesdir); \
	done

ifdef gnomeprefix
	$(INSTALL) -m 755 -d $(DESTDIR)$(datadir)/applications $(DESTDIR)$(pixmapdir)
	$(CAT) Goat.desktop | $(SED)  -e "s,%APP%,$(bindir)/$(PROG_EXE)," > Goat.tmp
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
