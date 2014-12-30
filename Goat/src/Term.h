// Goat - Scintilla based Text Editor
/** @file Term.h
 ** Interface to TERM facilities.
 **/
// Copyright 2011-2015 by Nunzio Raciti <nunzio.raciti@gmail.com>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef TERM_H_
#define TERM_H_

#define TERM_COL 			80
#define TERM_ROW 			25

#define TERM_MAX_COL 		255
#define TERM_MAX_ROW 		40
#define TERM_MIN_COL 		40
#define TERM_MIN_ROW 		15

#define TERM_ESC			0x1b
#define TERM_BS				8
#define TERM_CR				13
#define TERM_LF				10
#define TERM_TAB			'\t'

#define TAB_SIZE			4

#define TERM_COL_FORE		30
#define TERM_COL_BACK		40

#define TERM_COL_BLACK 		0
#define TERM_COL_RED   		1
#define TERM_COL_GREEN   	2
#define TERM_COL_YELLOW  	3
#define TERM_COL_BLUE   	4
#define TERM_COL_MAGENTA  	5
#define TERM_COL_CYAN  		6
#define TERM_COL_WHITE 		7

#define TERM_BUF_SIZE		(4096)
extern 	UART  	*serial;
extern	GUI::ScintillaWindow *pWin;

#define STATE_NORMAL 	0
#define STATE_ESCAPE 	1
#define STATE_CSI		2
#define STATE_EXT1  	3
#define STATE_EXT2  	4


class Term {
protected:

	char 	emptyLine[TERM_MAX_COL + 2];
	int 	position;
	int 	fontSize;
	bool 	termActive;
	int 	currentStyle;
	int 	state;
	int     col;
	int     row;
	char  	*buffer;

public:

	Term();
	~Term();
	void Clear();
	void ProcessAnsi(char byte);
	void DelChars(int posStart, int posEnd);
	void CleanChars(int posStart, int posEnd);
	int  PositionFromLine(int line);
	void SetTermWin(GUI::ScintillaWindow *p, int columns, int lines);
	void SetSerial(UART *uart) { serial = uart; };
	void SetAnsiColor(int ansiForeColor, int ansiBackColor, int ansiAttrib);
	bool isTermActive() {return termActive; };
	void setTermActiveOn() { termActive = true; };
	void setTermActiveOff() { termActive = false; };
	static int ProcessChars(void *data);
};

#endif /* TERM_H_ */
