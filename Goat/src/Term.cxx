/*
 * Term.cxx
 *
 *  Created on: 09/dic/2011
 *      Author: nuccio
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <time.h>
#include <assert.h>

#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include "Scintilla.h"
#include "SciLexer.h"
#include "ILexer.h"
#include "GUI.h"

#include "Goat.h"
#include "Mutex.h"
#include "Uart.h"
#include "Term.h"

//#define TERM_DEBUG 1

#ifdef TERM_DEBUG
#define SPACE_CHAR '@'
#define START_LINE 1
#define TDEBUG(__f, ...) printf(__f, ## __VA_ARGS__ )
#else
#define SPACE_CHAR ' '
#define START_LINE 0
#define TDEBUG(__f, ...)
#endif

UART  					*serial;
GUI::ScintillaWindow 	*pWin;
Term 					*term;

typedef long Colour;
inline Colour ColourRGB(unsigned int red, unsigned int green, unsigned int blue) {
	return red | (green << 8) | (blue << 16);
}

Term::Term() {
	pWin = 0;
	serial = 0;
	position = 0;
	termActive = false;
	col = TERM_COL;
	row = TERM_ROW;
	fontSize = 10;
	currentStyle = 0;
	state = STATE_NORMAL;
	buffer1 = (char *) malloc(TERM_BUF_SIZE+1);
	buffer2 = (char *) malloc(TERM_BUF_SIZE+1);
	term = this;
}

Term::~Term() {
	pWin = 0;
	position = 0;
	if (buffer1 != 0) {
		free(buffer1);
		buffer1 = 0;
	}
	if (buffer2 != 0) {
		free(buffer2);
		buffer1 = 0;
	}
}

void Term::SetTermWin(GUI::ScintillaWindow *w, int columns, int lines) {
	pWin = w;

	if (columns > TERM_MAX_COL || columns < TERM_MIN_COL  )
		col = TERM_COL;
	else
		col = columns;

	if (lines > TERM_MAX_ROW || columns < TERM_MIN_ROW  )
		row = TERM_ROW;
	else
		row = lines;
}

inline Colour GetColour(int ansiColor) {

	switch (ansiColor) {
	case 0: // Black
		return ColourRGB(0x00, 0x00, 0x00);
		break;
	case 1: // Red
		return ColourRGB(0xFF, 0x00, 0x00);
		break;
	case 2: // Green
		return ColourRGB(0x00, 0xFF, 0x00);
		break;
	case 3: // Yellow
		return ColourRGB(0xFF, 0xFF, 0x00);
		break;
	case 4: // Blue
		return ColourRGB(0x00, 0x00, 0xFF);
		break;
	case 5: // Magenta
		return ColourRGB(0xFF, 0x00, 0xFF);
		break;
	case 6: // Cyan
		return ColourRGB(0x00, 0xFF, 0xFF);
		break;
	case 7: // White
		return ColourRGB(0xFF, 0xFF, 0xFF);
		break;
	default:
		return ColourRGB(0xFF, 0xFF, 0xFF);
		break;
	}
	return 0;
}

void Term::SetAnsiColor(int ansiForeColor, int ansiBackColor, int ansiAttrib) {
	unsigned char style;

	if (ansiForeColor >= TERM_COL_FORE && ansiForeColor <= (TERM_COL_FORE + TERM_COL_WHITE))
		ansiForeColor = ansiForeColor - TERM_COL_FORE;
	else
		ansiForeColor = TERM_COL_WHITE;

	if (ansiBackColor >= TERM_COL_BACK && ansiBackColor <= (TERM_COL_BACK + TERM_COL_WHITE))
		ansiBackColor = ansiBackColor - TERM_COL_BACK;
	else
		ansiBackColor = TERM_COL_BLACK;

	style = (ansiForeColor * ansiBackColor) + ansiForeColor ;

	pWin->Call(SCI_STYLESETSIZE, style, fontSize);
	pWin->Send(SCI_STYLESETBACK, style, GetColour(ansiBackColor));
	pWin->Call(SCI_STYLESETFORE, style, GetColour(ansiForeColor));
	currentStyle = style;
}

void Term::Clear() {

	if (pWin == 0)
		return;

	pWin->Call(SCI_CLEARALL);
	position = 0;

	pWin->Call(SCI_SETVIEWEOL,0);
	pWin->Call(SCI_SETMARGINWIDTHN, 1, 0);
	pWin->Call(SCI_STYLERESETDEFAULT, 0, 0);
	pWin->Call(SCI_STYLESETFONT, STYLE_DEFAULT,  reinterpret_cast<uptr_t>("!monospace"));
	pWin->Call(SCI_SETCODEPAGE, SC_CP_UTF8, 0);

	if (isTermActive()) {
		pWin->Call(SCI_SETSTYLEBITS, 7, 0);
		pWin->Call(SCI_STYLESETBACK, STYLE_DEFAULT, ColourRGB(0x00, 0x00, 0x00));
		pWin->Call(SCI_STYLESETFORE, STYLE_DEFAULT, ColourRGB(0xff, 0xff, 0xff));
		pWin->Call(SCI_STYLESETSIZE, STYLE_DEFAULT, fontSize);
		pWin->Call(SCI_STYLECLEARALL, 0, 0);

		for (int cc=0; cc < col ; cc++) {
			emptyLine[cc] = SPACE_CHAR;
		}
		emptyLine[col] = '\n';

#ifdef TERM_DEBUG
		pWin->Call(SCI_SETVIEWEOL,1);
		pWin->Call(SCI_APPENDTEXT,col + 1,reinterpret_cast<sptr_t>("01234567890123456789012345678901234567890123456789012345678901234567890123456789\n"));
#endif
		for (int cc=START_LINE; cc < row; cc++) {
			pWin->Call(SCI_APPENDTEXT,col + 1,reinterpret_cast<sptr_t>(emptyLine));
		}
		pWin->Call(SCI_GOTOPOS, position);
		SetAnsiColor(TERM_COL_FORE+TERM_COL_WHITE, TERM_COL_BACK+TERM_COL_BLACK,0);

	} else {

		pWin->Call(SCI_AUTOCSETIGNORECASE, 1);
		pWin->Call(SCI_STYLESETBACK, STYLE_DEFAULT, ColourRGB(0xff, 0xff, 0xff));
		pWin->Call(SCI_STYLESETFORE, STYLE_DEFAULT, ColourRGB(0x00, 0x00, 0x00));
		pWin->Call(SCI_STYLESETSIZE, STYLE_DEFAULT, fontSize);
		pWin->Call(SCI_STYLECLEARALL, 0, 0);
	}
}

int Term::PositionFromLine(int line) {
	int new_line = pWin->Send(SCI_LINEFROMPOSITION, position) + line;

	if (new_line  < 0) new_line = 0;

	// It returns the document position that corresponds with the start of the line
	return pWin->Send(SCI_POSITIONFROMLINE, new_line);
}

void Term::DelChars(int posStart, int posEnd) {
	position = posStart;
	pWin->Send(SCI_SETTARGETSTART, posStart);
	pWin->Send(SCI_SETTARGETEND, posEnd);
	pWin->Send(SCI_REPLACETARGET, 0 , reinterpret_cast<sptr_t>(""));
	pWin->Send(SCI_GOTOPOS, position);
}

void Term::CleanChars(int posStart, int posEnd) {
	position = posStart;
	pWin->Send(SCI_SETTARGETSTART, posStart);
	pWin->Send(SCI_SETTARGETEND, posEnd);
	pWin->Send(SCI_REPLACETARGET, posEnd -posStart, reinterpret_cast<sptr_t>(emptyLine));
	pWin->Send(SCI_GOTOPOS, position);
}

void Term::ProcessAnsi(char byte) {
	static int number = 0;
	static int num[3];

	if (byte == TERM_ESC) {
		state = STATE_ESCAPE;
		number = 0;
	} else if (state == STATE_ESCAPE  && byte == '[') {
		state = STATE_CSI;
		number = 0;
	} else if (state == STATE_CSI && byte == 'K') {
		int end_line_pos = PositionFromLine(1) - 1;
		state = STATE_NORMAL;
		number = 0;
		TDEBUG("\n clear Line, %d,%d\n",position,end_line_pos);
		CleanChars(position , end_line_pos);
	} else if ((state == STATE_CSI || state == STATE_EXT1 || state == STATE_EXT2)
	        && byte >= '0' && byte <= '9') {
		number = (10 * number) + byte -'0';
	} else if (state == STATE_CSI && number == 2 && byte == 'J') {
		TDEBUG("Clear Screen\n");
		number = 0;
		state = STATE_NORMAL;
		Clear();
	} else if (state == STATE_CSI && number > 0 && byte == 'A') {
		state = STATE_NORMAL;
		// Move cursor up "number" lines
		TDEBUG("Move cursor up %u\n",number);
		position -= (number *(col + 1));
		number = 0;
	} else if (state == STATE_CSI && number > 0 && byte == 'B') {
		state = STATE_NORMAL;
		// Move cursor down "number" lines
		TDEBUG("Move cursor down %u\n",number);
		position += (number * (col + 1));
		number = 0;
	} else if (state == STATE_CSI && number > 0 && byte == 'C') {
		state = STATE_NORMAL;
		// Move cursor right "number" lines
		TDEBUG("Move cursor right %u\n",number);
		position += number;
		number = 0;
	} else if (state == STATE_CSI && number > 0 && byte == 'D') {
		state = STATE_NORMAL;
		// Move cursor left "number" lines
		TDEBUG("Move cursor left %u\n",number);
		position -= number;
		number = 0;
	} else if (state == STATE_CSI && byte == ';') {
		state = STATE_EXT1;
		num[0] = number;
		number = 0;
	} else if (state == STATE_EXT1 && byte == ';') {
		state = STATE_EXT2;
		num[1] = number;
		number = 0;
	} else 	if (state == STATE_EXT1 && byte == 'H') {
		num[1] = number;
		number = 0;
		state = STATE_NORMAL;
		if (num[0] > row) num[0]= row;
		if (num[0] < 1) num[0] = 1;
		if (num[1] > col) num[1] = col;
		if (num[1] < 1) num[1] = 1;
		position = pWin->Send(SCI_POSITIONFROMLINE, (num[0] -1)) + (num[1] - 1);
		TDEBUG("Move cursor to (x=%d, y=%d,position=%d)\n",num[1]-1,num[0]-1,position);
	} else if (state == STATE_EXT1 && byte == 'm') {
		state = STATE_NORMAL;
		num[1] = number;
		number = 0;
		SetAnsiColor(num[0],num[1],0);
	} else if (state == STATE_NORMAL && byte == TERM_CR) { /* \r */
		int current_line = pWin->Send(SCI_LINEFROMPOSITION, position);
		number = 0;
		position 		 = pWin->Send(SCI_POSITIONFROMLINE, current_line);
	} else if (state == STATE_NORMAL && byte == TERM_LF) { /* \n */
		int current_line 		= pWin->Send(SCI_LINEFROMPOSITION, position);
		int pos_on_next_line 	= PositionFromLine(1);
		number = 0;
		TDEBUG("Line feed\n");
		if (current_line >= (row -1)) {
			current_line=0;
			Clear();
		} else {
			position = pos_on_next_line;
		}
		pWin->Send(SCI_GOTOPOS, position);
	} else if (state == STATE_NORMAL && byte == TERM_TAB) {
		int pos_on_next_line = PositionFromLine(1);
		number = 0;
		if ((position + TAB_SIZE)  >= (pos_on_next_line - 1))
			position = pos_on_next_line;
		else
			position += TAB_SIZE;
		pWin->Send(SCI_GOTOPOS, position);
	} else  {
		int pos_on_next_line = PositionFromLine(1);
		char buf[2];
		number = 0;

		if (position >= (pos_on_next_line - 1))
			position = pos_on_next_line;

		DelChars(position, position + 1);
		buf[0] = byte;
		buf[1] = static_cast<char>(currentStyle);
		pWin->Send(SCI_GOTOPOS, position);
		pWin->Send(SCI_ADDSTYLEDTEXT, (1+1), reinterpret_cast<sptr_t>(buf));
		position++;
		pWin->Send(SCI_GOTOPOS, position);
		TDEBUG("%c",byte);
	}
}

int Term::ProcessChars(void *data) {
	int in,out,num_bytes;
	int line, lineStart;
	char byte;

#ifdef GTK
	/* This msSleep() here, keeps the cpu usage low :-o */
	msSleep(1);
#endif

	if (serial->IsXmodemOn()) return true;

	if (term->isTermActive()) {
		while (term->isTermActive() && !serial->RingBufferIsEmpty()) {
			if ((byte = serial->GetCharFromBuffer()) <= 0xff)
				term->ProcessAnsi(byte);
		}
	}

	if (!term->isTermActive()) {
		num_bytes = serial->GetCopyOfBuffer(term->buffer1, TERM_BUF_SIZE);

		if (pWin == 0 || num_bytes  == 0) return true;

		for (in=0,out=0; in < num_bytes; in++) {
			term->buffer2[out] = term->buffer1[in];
			if (term->buffer1[in] != TERM_CR  /* \r */)
				out++;
		}

		pWin->Call(SCI_HOME,0,0);
		pWin->Call(SCI_APPENDTEXT, out, reinterpret_cast<sptr_t>(term->buffer2));
		line = pWin->Call(SCI_GETLINECOUNT);
		lineStart = pWin->Call(SCI_POSITIONFROMLINE, line);
		pWin->Call(SCI_GOTOPOS, lineStart);
	}

	return true;
}
