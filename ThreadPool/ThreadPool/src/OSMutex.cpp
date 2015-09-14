#include "OSMutex.h"

OSMutex::OSMutex(void)
{
	lib_mutex_init(&m_mutex);
}

OSMutex::~OSMutex(void)
{
}

bool OSMutex::lock()
{
	lib_mutex_lock(&m_mutex);
	return true;
}

bool OSMutex::unlock()
{
	lib_mutex_unlock(&m_mutex);
	return true;
}
