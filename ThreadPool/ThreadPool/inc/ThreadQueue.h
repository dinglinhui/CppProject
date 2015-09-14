#ifndef THREADQUEUE_H_
#define THREADQUEUE_H_

#include <deque>
#include "OSMutex.h"
using namespace std;

class OSThread;
class CThreadQueue
{
public:
	CThreadQueue(void);
	~CThreadQueue(void);

public:
	OSThread *pop();
	bool push(OSThread *t);
	bool pushFront(OSThread *t); //²åµ½¶ÓÊ×¡£
	bool isEmpty();
	bool clear();

private:
	deque<OSThread *> m_TaskQueue;
	OSMutex m_mutex;
};
#endif
