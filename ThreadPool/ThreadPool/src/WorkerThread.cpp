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
			m_pTask->Start(); //ִ������
			//delete m_pTask; //�û�����Ŀռ佻���û������ڲ���������OSThread�Ӷ�ջ���䣬�˴��������⡣
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

//������������߳��ࡣ
bool CWorkerThread::AssignTask(OSThread *pTask)
{
	assert(pTask);

	m_pTask = pTask;

	return true;
}
