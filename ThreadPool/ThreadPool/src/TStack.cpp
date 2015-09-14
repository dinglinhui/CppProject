/*
 * CTStack.cpp
 *
 *  Created on: May 30, 2014
 *      Author: kding
 */

#include "TStack.h"

template<class T>
CTStack<T>::CTStack()
{

}

template<class T>
CTStack<T>::~CTStack()
{
}

template<class T>
T CTStack<T>::pop()
{
	m_mutext.lock();
	if (!m_stack.empty())
	{
		T t = m_stack.top();
		m_stack.pop();
		m_mutext.unlock();
		return t;
	}
	m_mutext.unlock();
	return NULL;
}

template<class T>
bool CTStack<T>::push(T t)
{
	assert(t);
	if (!t)
		return false;

	m_mutext.lock();
	m_stack.push(t);
	m_mutext.unlock();
	return true;
}

template<class T>
int CTStack<T>::getSize()
{
	m_mutext.lock();
	int size = m_stack.size();
	m_mutext.unlock();
	return size;
}

template<class T>
bool CTStack<T>::isEmpty()
{
	m_mutext.lock();
	bool ret = m_stack.empty();
	m_mutext.unlock();
	return ret;
}

template<class T>
bool CTStack<T>::clear()
{
	m_mutext.lock();
	m_stack.clear();
	m_mutext.unlock();
	return true;
}
