#ifndef __GOAT_UART_H_
#define __GOAT_UART_H_

#ifdef WIN32
#include <windows.h>
#include <commctrl.h>
#define DEF_PORT_NAME "COM1"
#else
#define DEF_PORT_NAME "/dev/ttyS0"

#include <termios.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <glib.h>
#include <gtk/gtk.h>
#endif
using namespace std;
#include <iostream>

#include "Scintilla.h"
#include "SciLexer.h"
#include "ILexer.h"
#include "GUI.h"
#include "Mutex.h"

#define NO_OPEN_PORT 		"No open port"
#define MAX_MESSAGE_SIZE	128
#define MAX_PORT_STR_SIZE 	64
#define BUFFER_RX 			8192
#define BUFFER2_RX 			4096
#define POLL_DELAY 			100    	/* in ms (for control signals) */
#define P_LOCK 				"/tmp"  /* lock file location */
#define MSG_WRN 			0
#define MSG_ERR 			1
#define MS_TIMEOUT 			3000

#define UART_EOL_CRLF 	0x15
#define UART_EOL_LF 	'\n'
#define UART_EOL_CR 	'\r'

#define UART_SEND_NO_FILE	0
#define UART_SEND_XMODEM	1
#define UART_SEND_RAW_REPL	2

extern	GUI::ScintillaWindow *pConWin;

class UART {
protected:
	GUI::ScintillaWindow  *pConWin;
	char 			buffer[BUFFER_RX + 1];
	Mutex 			*mutex;
	char 			buffer2[BUFFER2_RX + 1];
	Mutex 			*mutex2;

#ifdef WIN32
	HANDLE portHandle;
#else
	struct termios 	termios_save;
	int 			portFd;
#endif
	char 			portName[MAX_PORT_STR_SIZE+1];
	char 			message[MAX_MESSAGE_SIZE+1];
	bool 			isAlive;
	bool            isStarted;
	char 			lockfile[128];
	int				sendFileStatus;
	unsigned int  	portSpeed;
	unsigned char 	numBits;
	unsigned char 	numStopBits;
	unsigned char 	portParity;
	int 			crReceived;
	int 			portFlow;       // 0 : none, 1 : Xon/Xoff, 2 : RTS/CTS
	char			serialEOL;
	bool 			portOpened;
	int 			indexWrite;
	int 			indexRead;
	int 			indexWrite2;
	int 			indexRead2;
	unsigned char   *sendFileBuf;
	size_t  		sendFileBufSize;

	int Tx(const char *string, int length = 1);
	char *mbasename(char *s, char *res, int reslen);
	int CreateLockFile(const char *portName);
	void RemoveLockFile(void);
	void ClosePort(void);
	void ClosePortAndRemoveLockFile(void);
	bool OpenPort(const char *portName = DEF_PORT_NAME);

	/* Xmodem stuff */
	unsigned short crc16_ccitt(const unsigned char *buf, int len);
	int check(int crc, const unsigned char *buf, int sz);
	void flushinput(void);

public:
	static UART 	*instance;


	UART();
	~UART();

	bool SendCrlf(void);
	int  Send(const char *string, int length);
	bool Start(void);
	bool Status(void);
	bool IsConnected() { return portOpened;	}
	bool IsStarted() { return isStarted; }
	void Stop(void);
	void SendBreak(void);
	void Read(const char *buf, unsigned int size);
	void *GetSerialPortFD();
	void SetEOL(const char eol = UART_EOL_LF);
	char GetEOL(void);
	void SetTargetConsoleWin(GUI::ScintillaWindow *w = 0);
	void SetSpeed(unsigned int n = 115200);
	void SetNumBits(unsigned char n = 8);
	void SetNumStop(unsigned char n = 1);
	void SetParity(unsigned char n = 0);
	void SetPort(const char *s = DEF_PORT_NAME);
	char *GetMessage(void);
	bool IsAlive(void) { return isAlive; }
	void KillMe(void) { isAlive = FALSE; Stop(); }
	int  GetCopyOfBuffer(char *s, int len);
	/* Xmodem stuff */
	int  InByte(unsigned short timeout);
	void OutByte(int byte);
	int  XmodemReceive(unsigned char *dest, int destsz);
	int  XmodemTransmit();
	void SetSendFileStatus(int status) { sendFileStatus = status; };
	int GetSendFileStatus() { return sendFileStatus; }
	int  RawReplTransmit();
	int  SerialTrasmitFile(const char *name, int type);
	/* Ring buffer stuff */
	int  GetCharFromBuffer();
	int  PeekCharFromBuffer();
	void PutCharToBuffer(char byte);
	bool RingBufferIsEmpty();
	int  ListenPort();
	/* Unfortunately the UART thread can't write on GUI window directly
	 * we need a second buffer so the "Term::ProcessChars()" thread will use it */
	void PutCharToBuffer2(char byte);
	void ClearRingBuffer2();
	bool RingBuffer2IsEmpty();
	int  GetCharFromBuffer2();

private:

};

#define synchronized(M)  for(Lock M##_lock = M; M##_lock; M##_lock.setUnlock())

#endif
