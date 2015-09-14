#include "OSThread.h"

OSThread::OSThread(int nPrio, int nStackSize) :
	m_nPrio(nPrio),
	m_nStackSize(nStackSize),
	m_nThreadId(-1),
	m_hThread(NULL)
{
}

OSThread::~OSThread(void)
{
}

int OSThread::Start()
{
	int ret = -1;

	if (m_nPrio == -1)
		return ret;

	if (m_nThreadId == -1)
	{
		/*创建线程*/
		ret = lib_task_create(NULL, m_nPrio, m_nStackSize, &m_hThread, (LIB_TASK_ROUTINE) OSThread::ThreadFunction, (void*) this);

		if (ret == 0)
		{
			//create task ok
			m_nThreadId = m_nPrio;
		}
	}

	return ret;
}

int OSThread::Stop(void)
{
	int ret = -1;
	/*删除线程*/
	if (m_nThreadId == m_nPrio)
	{
		lib_task_exit(ret);
		m_nThreadId = -1;
	}

	return ret;
}

int OSThread::Pause(void)
{
	int ret = -1;
	/*暂停线程*/
	//if (m_nThreadId == m_nPrio)
	//	ret = lib_task_join(m_pThreadId, (void **) (int *) &ret);
	return ret;
}

int OSThread::Continue(void)
{
	int ret = 0;
	/*继续线程*/
	return ret;
}

void OSThread::ThreadFunction(void *pParam)
{
	OSThread* pThis = (OSThread *) pParam;
	if (!pThis)
		return;

	pThis->Run();
	pThis->Stop();
}
