/*
 * GMTask.h
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#ifndef INTERMEDIUM_GMET_INC_GMTASK_H_
#define INTERMEDIUM_GMET_INC_GMTASK_H_

#include "Command.h"

namespace pcols {

///////////////////////////////////////////////////////////////////////////////
// Class definied in other files
///////////////////////////////////////////////////////////////////////////////

class CGMPoint;
class CGMScheduler;

///////////////////////////////////////////////////////////////////////////////
// CGMTask
///////////////////////////////////////////////////////////////////////////////

class CGMTask {
public:
	CGMTask(void);
	virtual ~CGMTask(void);

	DWORD Execute(CGMScheduler *ptr);

	CGMTask* m_pNext;
	WORD m_nTN;
	void* m_pBody;
	void* m_pSrce;

	static void* operator new(size_t nSize);
	static void operator delete(void *p, size_t nSize);

private:
	void Clear(void);
	WORD AppendToHead(Command **lst, Command *cmd);

	int m_nNums;
	int m_nSend;
	int m_nRecv;
	int m_nErrs;
	int m_nTotalRecv;
	Command* m_pCommand;
	CGMPoint* m_pGMPoint;
};
} /* namespace pcols */

#endif /* INTERMEDIUM_GMET_INC_GMTASK_H_ */
