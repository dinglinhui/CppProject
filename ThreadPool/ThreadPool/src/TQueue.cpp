/*
 * CTQueue.cpp
 *
 *  Created on: May 30, 2014
 *      Author: kding
 */

#include "TQueue.h"

template<class T>
CTQueue<T>::CTQueue()
{

}

template<class T>
CTQueue<T>::~CTQueue()
{
}

template<class T>
T CTQueue<T>::pop()
{
	T p = NULL;
	m_mutex.lock();
	if (!m_TaskQueue.empty())
	{
		p = m_TaskQueue.front();
		m_TaskQueue.pop_front();
	}
	m_mutex.unlock();
	return p;
}

template<class T>
bool CTQueue<T>::push(T t)
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

template<class T>
bool CTQueue<T>::pushFront(T t)
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

template<class T>
bool CTQueue<T>::isEmpty()
{
	bool ret = false;
	m_mutex.lock();
	ret = m_TaskQueue.empty();
	m_mutex.unlock();
	return ret;
}

template<class T>
bool CTQueue<T>::clear()
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
