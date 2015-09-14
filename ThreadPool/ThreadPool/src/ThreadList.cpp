#include <cassert>
#include "ThreadList.h"
#include "WorkerThread.h"

CThreadList::CThreadList(void)
{
}

CThreadList::~CThreadList(void)
{
}

bool CThreadList::addThread(CWorkerThread *t)
{
	assert(t);
	if (!t)
		return false;

	m_mutex.lock();
	m_list.push_back(t);
	m_mutex.unlock();
	return true;
}

bool CThreadList::removeThread(CWorkerThread *t)
{
// 	std::list<CThread*>::iterator iter=m_list.begin();
// 	for(iter;iter!=m_list.end();iter++)
// 	{
// 		if(*iter==t)
// 		{
// 			break;
// 		}
// 	}

	assert(t);
	if (!t)
		return false;

	m_mutex.lock();
	m_list.remove(t);
	m_mutex.unlock();
	return true;
}

int CThreadList::getSize()
{
	m_mutex.lock();
	int size = m_list.size();
	m_mutex.unlock();
	return size;
}

bool CThreadList::isEmpty()
{
	m_mutex.lock();
	bool ret = m_list.empty();
	m_mutex.unlock();
	return ret;
}

bool CThreadList::clear()
{
	m_mutex.lock();
	/*m_list.splice(m_list.begin(), m_list);
	 std::list<CWorkerThread*>::iterator iter = m_list.begin();
	 for (; iter != m_list.end(); iter++)
	 {
	 delete (*iter);
	 }*/
	m_list.clear();
	m_mutex.unlock();
	return true;
}
