/*
 * CTStack.h
 *
 *  Created on: May 30, 2014
 *      Author: kding
 */

#ifndef CTStack_H_
#define CTStack_H_

#include <stack>
#include "OSMutex.h"
using namespace std;

template<class T>
class CTStack
{
public:
	CTStack();
	virtual ~CTStack();

public:
	T pop();
	bool push(T);
	int getSize();
	bool isEmpty();
	bool clear();

private:
	stack<T> m_stack;
	OSMutex m_mutext;
};

#endif /* CTStack_H_ */
