#ifndef THREADSTACK_H_
#define THREADSTACK_H_

#include <stack>
#include "OSMutex.h"

class CWorkerThread;
class CThreadStack
{
public:
	CThreadStack(void);
	~CThreadStack(void);

public:
	CWorkerThread* pop();
	bool push(CWorkerThread*);
	int getSize();
	bool isEmpty();
	bool clear();

private:
	std::stack<CWorkerThread*> m_stack;
	OSMutex m_mutext;
};

#endif
