## Go @ IDE is an editor dedicated to the embedded World

some features:

* Its editor (which is based on the wonderful library "scintilla" http://www.scintilla.org/), is lightweight and powerful.
* Has additional features such as "syntax highlighting", "code folding" and "code completion" based on the used language API.
* Includes the capability to connect with the real hardware (through the serial or the USB port) obtaining the console of the Target inside it.
* It's able to run the .py file on the Hardware (using the raw REPL interface).
* It's able to send the .lua/.lc file on the Hardware (using the XMODEM protocol).
* Supports some terminal commands (standard ansi/vt100), so it's able to play games like "hanoi" or "hungman" (using the TERM capability).
* Supports the FLASH (on the target) programming through the DFU/Arduino boot loader.
* Supports easily the "libusb" through Lua scripts (experimental).
* It's easily extensible through Lua scripts.

## Goat currently includes the support for:
* MicroPython http://micropython.org/
* eLua        http://www.eluaproject.net/
* Arduino     http://www.arduino.cc (experimental) the installation of the "Arduino IDE 1.0" is required.


## eLua Videos:
* http://www.youtube.com/watch?v=kUmujKemFjA
* http://www.youtube.com/watch?v=Z3g2Hu_FC3Q

## How to build and install on Linux:

* Be sure we have the package "libgtk+2.0" installed: 
* $ sudo apt-get install libgtk+2.0
* In order to support the "libluausb" (experimental) the libusb1.0 library is required:
* $ sudo apt-get install libusb-1.0
* $ cd $HOME
* $ git clone git://github.com/nuraci/Goat-IDE.git
* $ cd Goat-IDE
* $ cd scintilla/gtk
* $ make all
* $ cd $HOME/Goat-IDE/Goat
* $ make all
* $ sudo make uninstall (if isn't the first installation)
* $ sudo make install

## How to install on Windows:

* Download it from here: https://app.box.com/s/a15aw3gxi6gf7ubptarz
* Install as a Windows application and run it.

## How to build and install on Windows:

* Install the "Mingw/Msys" packages:
*    http://sourceforge.net/projects/mingw/files/latest/download?source=files
* Launch the setup "mingw-get-inst-xxxxxxxx.exe" and choice the "Download
  latest repository catalogues"
* Selecting following components: C and C++ compilers, "MSYS Basic System" and 
*    "Mingw Developer ToolKit".
* Donwnload Go@ sources from here: 
*     https://github.com/nuraci/Goat-IDE/zipball/master
* Now unzip sources inside a directory for example inside the c:\temp directory
* Open the mingw shell and go on sources directory:
* $ cd /c/temp/nuraci-Goat-IDE-xxxxxxx
* Build the scintilla library:
* $ cd /c/temp/nuraci-Goat-IDE-xxxxxxx/scintilla/win32
* $ make
* Build Goat:
* $ cd /c/temp/nuraci-Goat-IDE-xxxxxxx/Goat/win32
* $ make
* Copy runtime libraries:
* $ cp /mingw/bin/libgcc_s_dw2-1.dll /c/temp/nuraci-Goat-IDE-xxxxxxx/Goat/bin/
* $ cp /mingw/bin/libstdc++-6.dll /c/temp/nuraci-Goat-IDE-xxxxxxx/Goat/bin/
* Launch the Go@ editor:
* Use the windows explorer, localise Goat.exe and double click on it.

## Quick start.
* If your Linux is "Ubuntu" you can find "Goat" on menu: applications for development
* Then after you have connected one target board, please set the serial port name on Goat, using the menu: "Options" -> ""Open User Options File", and put this line inside it:
* serial.port=<serial_port>
*                              Windows esample: serial.port=COM6
*                              Linux   example: serial.port=/dev/ttyACM0
* save the "GoatUser.properties" (Windows) or the ".GoatUser.properties" (Linux) file
* After each change to the settings, you need to exit and reopen Goat again...
* Now you should have the target prompt on the "Target Console" window.
* Well, try to use the menu: "Open Example" and load some program (eg. helloworld.py or hello.lua)
* The last step, press "F5" or menu "Tools" -> "Run" will send the program directly on memory of the target board and will run it.
* Try it and please report problems and suggestions.

(*) just a note, some examples ("hanoi" and "hungman") requiring a special configuration of the "Target Console" window, so don't forget to select the menu: "Tools" -> "Mode Terminal".

Enjoy with the "Go @ IDE" and thank you for testing, improving and leaving your feedback for it.

## Note for MicroPython users
* the micropython firmware for the pyb board is here: http://micropython.org/download/
* the "firmware.bin", useful to install MP on the "Nucleo STM32F401" is available here: https://app.box.com/s/a15aw3gxi6gf7ubptarz

## Note for eLua users

* Target boards that running eLua firmware should be built using some optional modules:

* #define BUILD_XMODEM
* #define BUILD_SHELL
* #define BUILD_TERM
and if it is avaliable on our hardware:
* #define BUILD_MMCFS

* Goat for windows will install a directory "usb-driver" here: "C:\Program Files (x86)\The Go @ IDE\usb-driver" which contains:

* a) the eLua Windows driver of the USB UART (...\dfu-driver directory).
* b) the Windows driver of the DFU Programmer (...\uart-driver directory).

nunzio.raciti(AT)gmail.com 

