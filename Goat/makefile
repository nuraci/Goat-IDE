
DEBUG?=0
PREFIX?=.
INSTALL_BIN=$(PREFIX)/bin

all:
ifeq ($(OS),Windows_NT)
	@echo Build Win32
	make -C win32 DEBUG=$(DEBUG)
	make -C LUAplugins/luausb DEBUG=$(DEBUG)
	make -C LUAplugins/luausb install
	make -C LUAplugins/LuaBitOp-1.0.1 -f Makefile.mingw DEBUG=$(DEBUG)
	make -C LUAplugins/LuaBitOp-1.0.1 -f Makefile.mingw install
else
	@echo Build GTK
	make -C gtk DEBUG=$(DEBUG) all
	make -C LUAplugins/luausb DEBUG=$(DEBUG) 
	make -C LUAplugins/luausb install
	make -C LUAplugins/LuaBitOp-1.0.1 DEBUG=$(DEBUG)
	make -C LUAplugins/LuaBitOp-1.0.1 install
endif

clean:
ifeq ($(OS),Windows_NT)
	@echo Clean Win32 stuff
	make -C win32 clean
	make -C LUAplugins/luausb clean
	make -C LUAplugins/LuaBitOp-1.0.1 -f Makefile.mingw clean
else
	@echo Clean GTK stuff
	make -C gtk clean
	make -C LUAplugins/luausb clean
	make -C LUAplugins/LuaBitOp-1.0.1 clean
endif

install: all
ifeq ($(OS),Windows_NT)
	make -C win32 install
else
	sudo make -C gtk install
endif


uninstall:
ifeq ($(OS),Windows_NT)
else
	sudo make -C gtk uninstall
endif

.PHONY: all clean install uninstall
