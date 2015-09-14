#ifndef OSTHREAD_H_
#define OSTHREAD_H_

#include "OSLib.h"
#include "OSDefs.h"

class OSThread
{
public:
	OSThread(int nPrio, int nStackSize = 512);
	virtual ~OSThread(void);

	virtual int Start();
	virtual int Stop();
	virtual int Pause();
	virtual int Continue();

//Override
protected:
//OS Thread main body to run
	virtual int Run() = 0;

	static void ThreadFunction(void *pParam);

public:
	int m_nPrio;
	int m_nStackSize;
	int m_nThreadId;
	HANDLE m_hThread;
};

#endif
