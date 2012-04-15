
PREFIX?=.
INSTALL_BIN=$(PREFIX)/bin

all:
ifeq ($(OS),Windows_NT)
	make -C win32 
	make -C LUAplugins/luausb 
	make -C LUAplugins/LuaBitOp-1.0.1 Makefile.mingw 
	make -C LUAplugins/LuaBitOp-1.0.1 Makefile.mingw install
else
	make -C gtk all
	make -C LUAplugins/luausb 
	make -C LUAplugins/luausb install
	make -C LUAplugins/LuaBitOp-1.0.1 
	make -C LUAplugins/LuaBitOp-1.0.1 install
endif

clean:
ifeq ($(OS),Windows_NT)
	make -C win32 clean
	make -C LUAplugins/luausb clean
	make -C LUAplugins/LuaBitOp-1.0.1 Makefile.mingw clean
else
	make -C gtk clean
	make -C LUAplugins/luausb clean
	make -C LUAplugins/LuaBitOp-1.0.1 clean
endif
	rm -f $(PREFIX)/bin/*.so $(PREFIX)/bin/*.dll

install:all
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

.PHONY:all clean install uninstall
