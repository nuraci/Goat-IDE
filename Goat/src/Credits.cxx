// SciTE - Scintilla based Text Editor
/** @file Credits.cxx
 ** Display a list of contributors in the about box.
 **/
// Copyright 1998-2011 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <string>
#include <vector>
#include <set>
#include <map>

#include "Scintilla.h"
#include "ILexer.h"

#include "GUI.h"
#include "SString.h"
#include "StringList.h"
#include "StringHelpers.h"
#include "FilePath.h"
#include "PropSetFile.h"
#include "StyleWriter.h"
#include "Extender.h"
#include "Goat.h"
#include "Mutex.h"
#include "JobQueue.h"

#include "Cookie.h"
#include "Worker.h"
#include "SciTEBase.h"

const char License [] = { "\nLicense:\n\
Copyright 2011-2015 by Nunzio Raciti <nunzio.raciti@gmail.com>\n\
All Rights Reserved.\n\n\
Permission to use, copy, modify, and distribute this software and its\n\
documentation for any purpose and without fee is hereby granted, \n\
provided that the above copyright notice appear in all copies and that\n\
both that copyright notice and this permission notice appear in \n\
supporting documentation.\n\n\
NUNZIO RACITI DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS\n\
SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY\n\
AND FITNESS, IN NO EVENT SHALL NUNZIO RACITI BE LIABLE FOR ANY\n\
SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES\n\
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,\n\
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER\n\
TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE\n\
OR PERFORMANCE OF THIS SOFTWARE.\n\n" }; 

// AddStyledText only called from About so static size buffer is OK
void AddStyledText(GUI::ScintillaWindow &wsci, const char *s, int attr) {
	char buf[2000];
	size_t len = strlen(s);
	for (size_t i = 0; i < len; i++) {
		buf[i*2] = s[i];
		buf[i*2 + 1] = static_cast<char>(attr);
	}
	wsci.SendPointer(SCI_ADDSTYLEDTEXT,
	        static_cast<int>(len*2), const_cast<char *>(buf));
}

void SetAboutStyle(GUI::ScintillaWindow &wsci, int style, Colour fore) {
	wsci.Send(SCI_STYLESETFORE, style, fore);
}

static void HackColour(int &n) {
	n += (rand() % 100) - 50;
	if (n > 0xE7)
		n = 0x60;
	if (n < 0)
		n = 0x80;
}

void SciTEBase::SetAboutMessage(GUI::ScintillaWindow &wsci, const char *appTitle) {
	if (wsci.Created()) {
		wsci.Send(SCI_SETSTYLEBITS, 7, 0);
		wsci.Send(SCI_STYLERESETDEFAULT, 0, 0);
		int fontSize;

#if defined(WIN32)
		wsci.Send(SCI_STYLESETFONT, STYLE_DEFAULT,
		        reinterpret_cast<uptr_t>("!Verdana"));
		fontSize = 10;
#endif
#if defined(GTK)
		wsci.Send(SCI_STYLESETFONT, STYLE_DEFAULT,
		        reinterpret_cast<uptr_t>("!Serif"));
		fontSize = 14;
#endif

		wsci.Send(SCI_SETCODEPAGE, SC_CP_UTF8, 0);

		wsci.Send(SCI_STYLESETSIZE, STYLE_DEFAULT, fontSize);
		wsci.Send(SCI_STYLESETBACK, STYLE_DEFAULT, ColourRGB(0xff, 0xff, 0xff));
		wsci.Send(SCI_STYLECLEARALL, 0, 0);

		wsci.Send(SCI_STYLESETSIZE, 0, fontSize + 4);
		AddStyledText(wsci, "                              ", 0);
		AddStyledText(wsci, appTitle, 0);
		AddStyledText(wsci, "\n", 0);

		SetAboutStyle(wsci, 1, ColourRGB(0, 0, 0));
		int trsSty = 5; // define the stylenumber to assign font for translators.
		SString translator = GetTranslationToAbout("TranslationCredit", false);
		SetAboutStyle(wsci, trsSty, ColourRGB(0, 0, 0));
		AddStyledText(wsci, GetTranslationToAbout("Version").c_str(), trsSty);
		AddStyledText(wsci, " " VERSION_GOAT " ", 1);
		AddStyledText(wsci," - " __DATE__ " " __TIME__ "\n", 1);
        SetAboutStyle(wsci, 2, ColourRGB(0, 0, 0));
		AddStyledText(wsci, GetTranslationToAbout("by").c_str(), trsSty);
		AddStyledText(wsci, " Nunzio Raciti  ", 1);
  	    AddStyledText(wsci, "(nunzio.raciti@gmail.com)\n", 1);
  	    SetAboutStyle(wsci, 2, ColourRGB(255, 0, 0));
		wsci.Send(SCI_STYLESETITALIC, 2, 1);
		AddStyledText(wsci, GetTranslationToAbout("\nInfinite thanks to").c_str(), trsSty);
		AddStyledText(wsci, " Neil Hodgson", 2);
		AddStyledText(wsci, " @ http://www.scintilla.org,\n", 4);
		AddStyledText(wsci, GetTranslationToAbout("for his valuable work on Scintilla and SciTE, as this software\n").c_str(), trsSty);
		AddStyledText(wsci, GetTranslationToAbout("is based on Scintilla and SciTE ").c_str(), trsSty);
		AddStyledText(wsci, GetTranslationToAbout("(version").c_str(), trsSty);
		AddStyledText(wsci, " " VERSION_SCITE ").\n", 1);
		AddStyledText(wsci,License,5);
		SetAboutStyle(wsci, 3, ColourRGB(0, 0, 0));
		wsci.Send(SCI_SETREADONLY, 1, 0);
	}
}
