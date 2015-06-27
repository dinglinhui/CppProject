/*
 * Command.cpp
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#include "Command.h"
#include <string>
//#include "MemPool.hpp"
//#include <OSCriticalSection.h>

namespace pcols {

///////////////////////////////////////////////////////////////////////////////
// Command
///////////////////////////////////////////////////////////////////////////////

Command::Command(void) :
		m_nAck(0), m_pNext(nullptr), m_pAck(nullptr) {
	memset(m_body, 0, 32);
}

void* Command::operator new(size_t nSize) {
	if (nSize != sizeof(Command)) {
		return nullptr;
	}

//	void *ptr = mempool_alloc(nSize);
//	return ptr;
	return nullptr;
}

void Command::operator delete(void *p, size_t nSize) {
	if (nSize != sizeof(Command)) {
		return;
	}
//	mempool_free(p, nSize);
}

} /* namespace pcols */
