//#include <OSDefs.h>
#include <stddef.h>
#include <ThreadPool.h>
#include <windef.h>
#include <cassert>
#include <iostream>

using namespace std;

CThreadPool::CThreadPool(int num) :
	m_nMaxWorkers(num)/*, m_pVirTask(NULL)*/
{
}

CThreadPool::~CThreadPool(void)
{
}

int CThreadPool::CreateWorker(void)
{
	return CreateWorker(m_nMaxWorkers);
}

int CThreadPool::CreateWorker(int nWorkerNums)
{
	for (int i = 0; i < nWorkerNums; ++i)
	{
		CWorkerThread *pWorker = new CWorkerThread(i, this);
		if (pWorker != NULL)
		{
			m_IdleThreadStack.push(pWorker);
			pWorker->Start();
		}
	}

	return 0;
}

//将工作线程从活动队列取出，放入空闲线程栈中。在取之前判断此时任务队列是否有任务。如任务队列为空时才挂起。否则从任务队列取任务继续执行。
bool CThreadPool::SwitchActiveThread(CWorkerThread *pWorker)
{
	if (!m_TaskQueue.isEmpty()) //任务队列不为空，继续取任务执行。
	{
		OSThread *pTask = NULL;
		pTask = m_TaskQueue.pop();
		if (pTask != NULL)
		{
			pWorker->AssignTask(pTask);
			pWorker->Continue();
			cout << "task execute :" << pTask->m_nPrio << endl;
		}
		else
		{
			cout << "pop error 2" << endl;
		}
	}
	else //任务队列为空，该线程挂起。
	{
//		m_ActiveThreadList.removeThread(pWorker);
		m_ActiveThreadList.remove(pWorker);
		m_IdleThreadStack.push(pWorker);
	}
	return true;
}

bool CThreadPool::AddTask(OSThread *pTask, PRIORITY priority)
{
	assert(pTask);

	//根据线程优先级添加任务
	if (priority == PRIORITY_HIGH)
	{
		m_TaskQueue.pushFront(pTask); //高优先级任务。
	}
	else
	{
		m_TaskQueue.push(pTask); //进入任务队列。
	}
	cout << "task:" << pTask->m_nPrio << endl;

	DispatchWorker();
	return TRUE;
}

bool CThreadPool::DispatchWorker(void)
{
	//分派空闲工作线程
	if (!m_IdleThreadStack.isEmpty()) //存在空闲线程。调用空闲线程处理任务。
	{
		CWorkerThread *pWorker = m_IdleThreadStack.pop();
		if (pWorker == NULL)
			return FALSE;

		OSThread *pTask = m_TaskQueue.pop();
		if (pTask != NULL)
		{
			pWorker->AssignTask(pTask);
			pWorker->Continue();
//			m_ActiveThreadList.addThread(pWorker);
			m_ActiveThreadList.add(pWorker);
			cout << "task execute :" << pTask->m_nPrio << endl;
		}
		else
		{
			cout << "pop error 1" << endl;
		}
	}
	else
	{
//		cout << "no Worker" << endl;
	}

	return TRUE;
}

bool CThreadPool::DestroyThreadPool(void)
{
	m_TaskQueue.clear();
	m_ActiveThreadList.clear();

//	m_IdleThreadStack.clear();
	CWorkerThread * pWorker = NULL;
	while (!m_IdleThreadStack.isEmpty())
	{
		pWorker = m_IdleThreadStack.pop();
		if (pWorker != NULL)
		{
			delete pWorker, pWorker = NULL;
		}
	}
	return true;
}
