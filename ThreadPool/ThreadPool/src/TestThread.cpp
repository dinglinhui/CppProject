#include "TestThread.h"

CTestThread::CTestThread(int nPrio, int nStackSize) :
	OSThread(nPrio, nStackSize)
{
}

CTestThread::~CTestThread(void)
{
}

int CTestThread::Run(void)
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 1000; j++)
		{
			int temp = 1;
			temp++;
		}
	}
	return 0;
}
