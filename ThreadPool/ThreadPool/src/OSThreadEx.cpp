/*
 * OSThreadEx.cpp
 *
 *  Created on: 2014Äê2ÔÂ28ÈÕ
 *      Author: dinglinhui
 */

#include "OSThreadEx.h"

OSThreadEx::OSThreadEx(int nPrio, int nStackSize/*, int nQueueBuffSize*/) :
	OSThread(nPrio, nStackSize),
	m_pNext(NULL)/*, OSMessageBase(nQueueBuffSize)*/
{

}

OSThreadEx::~OSThreadEx()
{
}

int OSThreadEx::Run(void)
{
	for (;;)
	{
		Sleep(1000);
		break;
	}
	return 0;
}
