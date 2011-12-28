// SciTE - Scintilla based Text Editor
/** @file GTKMutex.cxx
 ** Define mutex
 **/
// SciTE & Scintilla copyright 1998-2003 by Neil Hodgson <neilh@scintilla.org>
// Copyright 2007 by Neil Hodgson <neilh@scintilla.org>, from April White <april_white@sympatico.ca>
// The License.txt file describes the conditions under which this software may be distributed.

// http://www.microsoft.com/msj/0797/win320797.aspx

#include <pthread.h>
#include "Mutex.h"

class GTKMutex : public Mutex {

private:
	pthread_mutex_t mutex;
	virtual void Lock() {pthread_mutex_lock( &mutex ); }
	virtual void Unlock() {pthread_mutex_unlock( &mutex ); }
	GTKMutex() {
		mutex = PTHREAD_MUTEX_INITIALIZER;
	}
	virtual ~GTKMutex() {}
	friend class Mutex;
};

Mutex *Mutex::Create() {
   return new GTKMutex();
}
