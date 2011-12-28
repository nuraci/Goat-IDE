## Go @ IDE is an editor dedicated to the eLua World: http://www.eluaproject.net/

some features:

* Its editor (which is based on the wonderful library "scintilla" http://www.scintilla.org/), is lightweight and powerful.
* It has additional features such as "syntax highlighting", "code folding" and "code completion" based on the eLua API.
* It includes the  capability to connect with the real hardware (through the serial or the USB port) obtaining the prompt of Elua inside it.
* Also it supports some terminal commands (standard ansi/vt100), so it's able to play games like "hanoi" or "hungman".



## How to build and install it on Linux:

* Be sure we have the package "libgtk+2.0" installed: 
* $ sudo apt-get install libgtk+2.0
* $ cd $HOME
* $ git clone git://github.com/nuccioraciti/Goat-IDE.git
* $ cd Goat-IDE
* $ cd scintilla/gtk
* $ make all
* $ cd $HOME/Goat-IDE
* $ cd Goat/gtk
* $ make all
* $ sudo make install

## How to build and install it on Windows:
Not yet ready, I hope that it will be soon...


## Quick start.
* If your Linux is "Ubuntu" you can find "Goat" on menu: applications for development
* Then after you have connected one eLua board, please set the serial port name on Goat, using the menu: "Options" -> "Open Global Options File", un-commenting the right choice for you system. 
* After each change to the settings, you need to exit and reopen Goat again...
* Now you should have the eLua prompt on the "eLua's Console" window.
* Well, try to use the menu: "Open Example" and load some elua program (eg. hello.lua)
* Using the menu: "Tools" -> "Compile", let's Goat build the program for you.
* The last step, press "F5" or menu "Tools" -> "Send & Run on Ram" will send the "hello.lc" directly on memory of eLua board and will run it.

(*) just a note, some eLua examples ("hanoi" and "hungman") requiring a special configuration of the "eLua Console" window, so don't forget to select the menu: "Tools" -> "Mode Terminal".

Enjoy with the "Go @ IDE" and thank you for testing, improving and leaving your feedback for it.

<raciti.nuccio(AT)gmail.com> <Mizar32@http://www.simplemachines.it >

