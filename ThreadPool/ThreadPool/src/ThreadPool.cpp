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

//�������̴߳ӻ����ȡ������������߳�ջ�С���ȡ֮ǰ�жϴ�ʱ��������Ƿ����������������Ϊ��ʱ�Ź��𡣷�����������ȡ�������ִ�С�
bool CThreadPool::SwitchActiveThread(CWorkerThread *pWorker)
{
	if (!m_TaskQueue.isEmpty()) //������в�Ϊ�գ�����ȡ����ִ�С�
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
	else //�������Ϊ�գ����̹߳���
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

	//�����߳����ȼ��������
	if (priority == PRIORITY_HIGH)
	{
		m_TaskQueue.pushFront(pTask); //�����ȼ�����
	}
	else
	{
		m_TaskQueue.push(pTask); //����������С�
	}
	cout << "task:" << pTask->m_nPrio << endl;

	DispatchWorker();
	return TRUE;
}

bool CThreadPool::DispatchWorker(void)
{
	//���ɿ��й����߳�
	if (!m_IdleThreadStack.isEmpty()) //���ڿ����̡߳����ÿ����̴߳�������
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
