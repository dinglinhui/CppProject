#ifndef WORKERTHREAD_H_
#define WORKERTHREAD_H_

#include "OSThread.h"
#include "ThreadPool.h"

class CBaseThreadPool;
class CWorkerThread: public OSThread
{
public:
	CWorkerThread(int id, CBaseThreadPool *threadPool);
	virtual ~CWorkerThread(void);

public:
	bool AssignTask(OSThread *pTask);

protected:
	virtual int Run(void);

private:
	OSThread *m_pTask; //��ǰ��������ָ��
	CBaseThreadPool *m_pThreadPool;
};
#endif
