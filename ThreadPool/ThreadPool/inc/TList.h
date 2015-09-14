/*
 * CTList.h
 *
 *  Created on: May 30, 2014
 *      Author: kding
 */

#ifndef TList_H_
#define TList_H_

#include <list>
#include "OSMutex.h"

template<class T>
class CTList
{
public:
	CTList();
	virtual ~CTList();

public:
	bool add(T t);
	bool remove(T t);

	bool isEmpty();
	bool clear();
	int getSize();

private:
	std::list<T> m_list;
	OSMutex m_mutex;
};

#endif /* CTList_H_ */
