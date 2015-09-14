#include <iostream>
#include "ThreadPool.h"
#include "WorkerThread.h"
#include "TestThread.h"
#include "OSThreadEx.h"

OSThreadEx* g_pThreadList = NULL;
CThreadPool *g_pThreadPool = NULL;

int Register(OSThreadEx *pService)
{
	OSThreadEx **ppPrev = &g_pThreadList;
	while (*ppPrev != NULL)
	{
		ppPrev = &(*ppPrev)->m_pNext;
	}

	pService->m_pNext = *ppPrev;
	*ppPrev = pService;

	return 0;
}

void RecycleThreads(void)
{
	OSThreadEx* pEntry = g_pThreadList;
	OSThreadEx* pEntryNext = NULL;

	while (pEntry != NULL)
	{
		//active task queue not null or active worker thread list is not null
		if ((!g_pThreadPool->m_TaskQueue.isEmpty()) || (!g_pThreadPool->m_ActiveThreadList.isEmpty()))
		{
			continue;
		}

		if (lib_task_join(pEntry->m_hThread, NULL) < 0)
		{
			cout << "task join failure!" << endl;
		}
		lib_task_detach(pEntry->m_hThread);

		pEntryNext = pEntry->m_pNext;

		delete pEntry, pEntry = NULL;
		pEntry = pEntryNext;
	}

	cout << "recycle threads success" << endl;
}

int main(int argc, char**argv)
{
	g_pThreadPool = new CThreadPool(10); //threadPool max worker thread numbers
	g_pThreadPool->CreateWorker();

	int i = 0;
	OSThreadEx *pex = NULL;
	for (; i < 15; i++)
	{
		pex = new OSThreadEx(i);
		Register(pex);
		g_pThreadPool->AddTask(pex, PRIORITY_NORMAL);
	}

	for (; i < 30; i++)
	{
		pex = new OSThreadEx(i);
		Register(pex);
		g_pThreadPool->AddTask(pex, PRIORITY_HIGH);
	}

	RecycleThreads();
	g_pThreadPool->DestroyThreadPool();
	if (g_pThreadPool != NULL)
		delete g_pThreadPool, g_pThreadPool = NULL;

	cout << "exit 0" << endl;
	return 0;
}
