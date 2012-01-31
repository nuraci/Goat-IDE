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

#define NO_OPEN_PORT 		"No open port"
#define MAX_MESSAGE_SIZE	128
#define MAX_PORT_STR_SIZE 	64
#define BUFFER_RX 			8192
#define POLL_DELAY 			100    	/* in ms (for control signals) */
#define P_LOCK 				"/tmp"  /* lock file location */
#define MSG_WRN 			0
#define MSG_ERR 			1

class UART  {
protected:
	char 			buffer[BUFFER_RX + 1];

#ifdef WIN32
	HANDLE portHandle;
#else
	struct termios 	termios_save;
	int 			portFd;
#endif
	char 			portName[MAX_PORT_STR_SIZE+1];
	char 			message[MAX_MESSAGE_SIZE+1];
	bool 			isAlive;
	char 			lockfile[128];
	bool			xmodemOn;
	unsigned int  	portSpeed;
	unsigned char 	numBits;
	unsigned char 	numStopBits;
	unsigned char 	portParity;
	int 			crReceived;
	int 			portFlow;       // 0 : none, 1 : Xon/Xoff, 2 : RTS/CTS
	bool 			clrfAuto;      	// line feed auto
	bool 			portOpened;
	int 			indexWrite;
	int 			indexRead;

	int Tx(const char *string, int length = 1);
	char *mbasename(char *s, char *res, int reslen);
	int CreateLockFile(const char *portName);
	void RemoveLockFile(void);
	void ClosePort(void);
	void ClosePortAndRemoveLockFile(void);
	bool OpenPort(const char *portName = DEF_PORT_NAME);
	bool SendCrlf(void);
	/* Xmodem stuff */
	unsigned short crc16_ccitt(const unsigned char *buf, int len);
	int check(int crc, const unsigned char *buf, int sz);
	void flushinput(void);

public:
	static UART 	*instance;
	Mutex *mutex;

	UART();
	~UART();

	int Send(const char *string, int length);
	bool Start(void);
	bool IsConnected() { return portOpened;	}
	void Stop(void);
	void ConfigureclrfAuto(bool clrfAuto);
	void SendBreak(void);
	void Read(const char *buf, unsigned int size);
	void *GetSerialPortFD();
	void SetSpeed(unsigned int n = 115200);
	void SetNumBits(unsigned char n = 8);
	void SetNumStop(unsigned char n = 1);
	void SetParity(unsigned char n = 0);
	void SetPort(const char *s = DEF_PORT_NAME);
	char *GetStartMessage() { return message; }
	bool IsAlive(void) { return isAlive; }
	void KillMe(void) { isAlive = FALSE; Stop(); }
	int GetCopyOfBuffer(char *s, int len);
	int GetCharFromBuffer();
	void  PutCharToBuffer(char byte);
	/* Xmodem stuff */
	int InByte(unsigned short timeout);
	void OutByte(int byte);
	int XmodemReceive(unsigned char *dest, int destsz);
	int XmodemTransmit(unsigned char *src, int srcsz);
	void XmodemOn(bool state) { xmodemOn = state;  };
	bool IsXmodemOn() { return xmodemOn; }
	int XmodemTransmitFile(const char *name);
	/* Ring buffer stuff */
	bool RingBufferIsEmpty();
	int ListenPort();

private:

};

#ifdef WIN32
#define FALSE 			0
#define TRUE 			1
#define KeepGuiLive()
#define waitMs(ms) { for (unsigned int a=0; a<ms ; a++) { msSleep(1);}}
DWORD WINAPI ListenPort(LPVOID lpParam);
#else
#define KeepGuiLive() { gtk_main_iteration_do(false); }
#define waitMs(ms) { for (unsigned int a=0; a<ms ; a++) { gtk_main_iteration_do (false); msSleep(1);}}
#endif

#define synchronized(M)  for(Lock M##_lock = M; M##_lock; M##_lock.setUnlock())

#endif
