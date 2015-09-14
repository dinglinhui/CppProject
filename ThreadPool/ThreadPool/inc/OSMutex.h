#ifndef OSMUTEX_H_
#define OSMUTEX_H_

#include "OSLib.h"

class OSMutex
{
public:
	OSMutex(void);
	~OSMutex(void);

public:
	bool lock();
	bool unlock();

private:
	pthread_mutex_t m_mutex;
};

#endif
