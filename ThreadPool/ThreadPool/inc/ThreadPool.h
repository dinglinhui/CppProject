#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <list>
#include "OSThread.h"
#include "WorkerThread.h"
#include "TList.cpp"
#include "TQueue.cpp"
#include "TStack.cpp"

class CWorkerThread;
class OSThread;

typedef enum TAG_PRIORITY
{
	PRIORITY_NORMAL,
	PRIORITY_HIGH
} PRIORITY;

class CBaseThreadPool
{
public:
	CBaseThreadPool(){}
	virtual ~CBaseThreadPool(){};
	virtual bool SwitchActiveThread(CWorkerThread *) = 0;
};

class CThreadPool: public CBaseThreadPool
{
public:
	CThreadPool(int num);
	virtual ~CThreadPool(void);

	int CreateWorker(void);
	int CreateWorker(int nWorkerNums);

public:
	virtual bool SwitchActiveThread(CWorkerThread *);

public:
	//priority为优先级。高优先级的任务将被插入到队首。
	bool AddTask(OSThread *pTask, PRIORITY priority);
	bool DispatchWorker(void);
	bool DestroyThreadPool(void);

	int m_nMaxWorkers;
	CTStack<CWorkerThread *>  m_IdleThreadStack; //worker thread idle stack
	CTList<CWorkerThread *> m_ActiveThreadList; //worker thread active list
	CTQueue<OSThread *> m_TaskQueue; //active task queue

};

#endif
