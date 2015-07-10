/*
 * Command.h
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#include <cstdlib>
#include "OSDefs.h"

namespace pcols {

///////////////////////////////////////////////////////////////////////////////
// Command
///////////////////////////////////////////////////////////////////////////////

class Command {
public:
	Command(void);

	int m_nAck;
	BYTE m_body[32];
	Command* m_pNext;
	Command* m_pAck;

	static void* operator new(size_t nSize);
	static void operator delete(void *p, size_t nSize);
};

} /* namespace pcols */

#endif /* COMMAND_H_ */
