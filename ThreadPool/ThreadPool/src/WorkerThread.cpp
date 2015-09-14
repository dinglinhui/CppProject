#include <cassert>

#include "OSThread.h"
#include "WorkerThread.h"
#include "ThreadPool.h"
#include <iostream>
using namespace std;

#define WAIT_TIME 20

CWorkerThread::CWorkerThread(int id, CBaseThreadPool *threadPool) :
	OSThread(id),
	m_pTask(NULL),
	m_pThreadPool(threadPool)
{
}

CWorkerThread::~CWorkerThread(void)
{
}

int CWorkerThread::Run(void)
{
	for (;;)
	{
		if (m_pTask)
		{
			m_pTask->Start(); //执行任务。
			//delete m_pTask; //用户传入的空间交由用户处理，内部不处理。如OSThread从堆栈分配，此处会有问题。
			m_pTask = NULL;
			m_pThreadPool->SwitchActiveThread(this);
		}
		else
		{
			//cout << "worker have no Task" << endl;
		}

		Sleep(50);
	}
	return 0;
}

//将任务关联到线程类。
bool CWorkerThread::AssignTask(OSThread *pTask)
{
	assert(pTask);

	m_pTask = pTask;

	return true;
}
