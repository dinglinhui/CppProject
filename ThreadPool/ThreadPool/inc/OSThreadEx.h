/*
 * OSThreadEx.h
 *
 *  Created on: 2014��2��28��
 *      Author: dinglinhui
 */
#ifndef OSTHREADEX_H_
#define OSTHREADEX_H_

#include "OSThread.h"
//#include "OSMessageBase.h"

class OSThreadEx: public OSThread //, public OSMessageBase
{
public:
	OSThreadEx(int nPrio, int nStackSize = 512/*, int nQueueBuffSize*/);
	virtual ~OSThreadEx();

	OSThreadEx *m_pNext;

protected:
	virtual int Run(void);
};
#endif
