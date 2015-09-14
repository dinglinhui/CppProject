#ifndef THREADLIST_H_
#define THREADLIST_H_

#include <list>
#include "OSMutex.h"

class CWorkerThread;
class CThreadList
{
public:
	CThreadList(void);
	~CThreadList(void);

public:
	bool addThread(CWorkerThread*t);
	bool removeThread(CWorkerThread*t);
	bool isEmpty();
	bool clear();
	int getSize();

private:
	std::list<CWorkerThread*> m_list;
	OSMutex m_mutex;
};

#endif
