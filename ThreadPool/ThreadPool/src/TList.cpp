/*
 * CTList.cpp
 *
 *  Created on: May 30, 2014
 *      Author: kding
 */
#include <cassert>
#include "TList.h"

template<class T>
CTList<T>::CTList()
{

}

template<class T>
CTList<T>::~CTList()
{

}

template<class T>
bool CTList<T>::add(T t)
{
	m_mutex.lock();
	m_list.push_back(t);
	m_mutex.unlock();
	return true;
}

template<class T>
bool CTList<T>::remove(T t)
{
	// 	std::list<CThread*>::iterator iter=m_list.begin();
	// 	for(iter;iter!=m_list.end();iter++)
	// 	{
	// 		if(*iter==t)
	// 		{
	// 			break;
	// 		}
	// 	}

	m_mutex.lock();
	m_list.remove(t);
	m_mutex.unlock();
	return true;
}

template<class T>
bool CTList<T>::isEmpty()
{
	m_mutex.lock();
	bool ret = m_list.empty();
	m_mutex.unlock();
	return ret;
}

template<class T>
bool CTList<T>::clear()
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

template<class T>
int CTList<T>::getSize()
{
	m_mutex.lock();
	int size = m_list.size();
	m_mutex.unlock();
	return size;
}
