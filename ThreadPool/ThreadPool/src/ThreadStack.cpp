#include <cassert>
#include "ThreadStack.h"
#include "WorkerThread.h"

CThreadStack::CThreadStack(void)
{
}

CThreadStack::~CThreadStack(void)
{
}

CWorkerThread* CThreadStack::pop()
{
	m_mutext.lock();
	if (!m_stack.empty())
	{
		CWorkerThread *t = m_stack.top();
		m_stack.pop();
		m_mutext.unlock();
		return t;
	}
	m_mutext.unlock();
	return NULL;
}

bool CThreadStack::push(CWorkerThread* t)
{
	assert(t);
	if (!t)
		return false;

	m_mutext.lock();
	m_stack.push(t);
	m_mutext.unlock();
	return true;
}

int CThreadStack::getSize()
{
	m_mutext.lock();
	int size = m_stack.size();
	m_mutext.unlock();
	return size;
}

bool CThreadStack::isEmpty()
{
	m_mutext.lock();
	bool ret = m_stack.empty();
	m_mutext.unlock();
	return ret;
}

bool CThreadStack::clear()
{
	m_mutext.lock();
	CWorkerThread *pThread = NULL;
	while (!m_stack.empty())
	{
		pThread = m_stack.top();
		m_stack.pop();
		pThread->Continue();
		delete pThread;
	}
	m_mutext.unlock();
	return true;
}
