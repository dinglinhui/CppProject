/*
 * TQueue.h
 *
 *  Created on: May 30, 2014
 *      Author: kding
 */

#ifndef TQUEUE_H_
#define TQUEUE_H_

#include <deque>
#include "OSMutex.h"
using namespace std;

template<class T>
class CTQueue
{
public:
	CTQueue();
	virtual ~CTQueue();

public:
	T pop();
	bool push(T t);
	bool pushFront(T t); //²åµ½¶ÓÊ×¡£
	bool isEmpty();
	bool clear();

private:
	deque<T> m_TaskQueue;
	OSMutex m_mutex;
};

#endif /* TQUEUE_H_ */
