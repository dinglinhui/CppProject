#ifndef TESTTHREAD_H_
#define TESTTHREAD_H_

#include "OSThread.h"

class CTestThread: public OSThread
{
public:
	CTestThread(int nPrio, int nStackSize);
	virtual ~CTestThread(void);

protected:
	virtual int Run(void);
};
#endif
