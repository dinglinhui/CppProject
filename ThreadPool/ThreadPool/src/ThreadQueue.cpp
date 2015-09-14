#include "ThreadQueue.h"
#include "OSThread.h"

CThreadQueue::CThreadQueue(void)
{
}

CThreadQueue::~CThreadQueue(void)
{
}

OSThread* CThreadQueue::pop()
{
	OSThread *p = NULL;
	m_mutex.lock();
	if (!m_TaskQueue.empty())
	{
		p = m_TaskQueue.front();
		m_TaskQueue.pop_front();
	}
	m_mutex.unlock();
	return p;
}

bool CThreadQueue::push(OSThread *t)
{
	if (t == NULL)
	{
		return false;
	}

	m_mutex.lock();
	m_TaskQueue.push_back(t);
	m_mutex.unlock();
	return true;
}


bool CThreadQueue::pushFront(OSThread *t)
{
	if (t == NULL)
	{
		return false;
	}

	m_mutex.lock();
	m_TaskQueue.push_front(t);
	m_mutex.unlock();
	return true;
}

bool CThreadQueue::isEmpty()
{
	bool ret = false;
	m_mutex.lock();
	ret = m_TaskQueue.empty();
	m_mutex.unlock();
	return ret;
}


bool CThreadQueue::clear()
{
	m_mutex.lock();
// 	std::deque<OSThread*>::iterator iter=m_TaskQueue.begin();
// 	for(;iter!=m_TaskQueue.end();iter++)
// 	{
// 		delete (*iter);
// 	}
	m_TaskQueue.clear();
	m_mutex.unlock();
	return true;
}
