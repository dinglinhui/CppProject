/*
 * GMTask.cpp
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#include "GMTask.h"
#include <cassert>
//#include <MemPool.h>
//#include <OSCriticalSection.h>
#include "GMScheduler.h"

namespace pcols {

///////////////////////////////////////////////////////////////////////////////
// Constant definitions
///////////////////////////////////////////////////////////////////////////////

#define GM_DEFAULT_ERRS	(8)		// 连续通信错误次数

///////////////////////////////////////////////////////////////////////////////
// CGMTask
///////////////////////////////////////////////////////////////////////////////

CGMTask::CGMTask(void) :
		m_pNext(nullptr),
		m_nTN(0),
		m_pBody(nullptr),
		m_pSrce(nullptr),
		m_nNums(0),
		m_nSend(0),
		m_nRecv(0),
		m_nErrs(0),
		m_nTotalRecv(0),
		m_pCommand(nullptr),
		m_pGMPoint(nullptr) {
}

CGMTask::~CGMTask(void) {
	Clear();
}

void* CGMTask::operator new(size_t nSize) {
	if (nSize != sizeof(CGMTask)) {
//		return nullptr;
		throw std::logic_error("size error");
	}

//	void *ptr = mempool_alloc(nSize);
//	return ptr;
	return nullptr;
}

void CGMTask::operator delete(void *p, size_t nSize) {
	if (nSize != sizeof(CGMTask)) {
		return;
	}
//	mempool_free(p, nSize);
}

void CGMTask::Clear(void) {
	Command *ptr = m_pCommand;
	Command *pxt = nullptr;

	while (ptr != nullptr) {
		pxt = ptr->m_pNext;
		delete ptr;
		ptr = pxt;
	}
	m_pCommand = nullptr;
}

WORD CGMTask::AppendToHead(Command **lst, Command *cmd) {
	int nNums = 1;
	Command *pLast = cmd;
	while ((pLast != nullptr) && (pLast->m_pNext != nullptr)) {
		nNums++;
		pLast = pLast->m_pNext;
	}
	assert(pLast != nullptr);
	assert(pLast->m_pNext == nullptr);

	pLast->m_pNext = (*lst);
	(*lst) = cmd;
	return nNums;
}

DWORD CGMTask::Execute(CGMScheduler *ptr) {
	assert(ptr != nullptr);

	// While the last measure point gather complete,
	// the task execute complete.
	if ((m_pGMPoint == nullptr) && (m_nSend != 0)) {
		GMRET ret = (m_nTotalRecv > 0) ? GM_OK : GM_ERROR;
		return MAKELONG(ret | GM_COMPLETE, m_nTN);
	}

	// Current measure point gather complete,
	// then gather next measure point.
	if ((m_pGMPoint != nullptr) && (m_pCommand == nullptr) && (m_nNums > 0)) {
		GMRET ret = (m_nRecv > 0) ? GM_OK : GM_ERROR;
		DWORD dwV = MAKELONG(ret | GM_POINT, m_pGMPoint->m_TN);

		if (m_nTN == 0xFFFF)
			m_pGMPoint = ptr->FindNextActive(m_pGMPoint);
		else
			m_pGMPoint = nullptr;
		m_nNums = 0;
		return dwV;
	}

	// If the task execute for the first time,
	// get first measure point object from list.
	if (m_pGMPoint == nullptr) {
		if (m_nTN == 0xFFFF)
			m_pGMPoint = ptr->FindFirstActive();
		else
			m_pGMPoint = ptr->Find(m_nTN);
	}

	// Invalid measure point,
	// the task will be canceled.
	if (m_pGMPoint == nullptr) {
		return MAKELONG(GM_EXCEPTION, m_nTN);
	}

	// If current measure point execute for the first time,
	// get all commands by relevant protocol.
	if (m_nNums <= 0) {
		m_nNums = m_pGMPoint->MakeCommands(m_pBody, m_pCommand);

		// No any commands
		if (m_nNums <= 0) {
			WORD tn = m_pGMPoint->m_TN;
			if (m_nTN == 0xFFFF)
				m_pGMPoint = ptr->FindNextActive(m_pGMPoint);
			else
				m_pGMPoint = nullptr;

			// 2007.9.9
			// raised GM_COMPLETE message when error happend
			m_nSend = -1;

			return MAKELONG(GM_ERROR | GM_POINT, tn);
		}

		m_nSend = 0;
		m_nRecv = 0;
		m_nErrs = 0;
	}

	if (m_pCommand == nullptr) {
		return MAKELONG(GM_EXCEPTION, m_nTN);
	}

	// Is a new session for measure point,
	// reset comdcb before gather.
	if (m_nSend == 0) {
		ptr->ResetComDcb(m_pGMPoint);
	}

	GMRET ret = GM_ERROR;

	// execute command
	Command *pCMD = m_pCommand;
	m_pCommand = m_pCommand->m_pNext;
	m_nSend++;

	if (ptr->RequestData(m_pGMPoint, pCMD) == 0) {
		m_nRecv++;
		m_nTotalRecv++;
		m_nErrs = 0;

		ret = GM_OK;
		if (pCMD->m_pAck != nullptr) {
			m_nNums += AppendToHead(&m_pCommand, pCMD->m_pAck);
		}
	} else {
		if ((++m_nErrs) >= GM_DEFAULT_ERRS) {
			Clear();
		}
	}
	delete pCMD, pCMD = nullptr;

	return MAKELONG(ret | GM_ITEM, m_pGMPoint->m_TN);
}

} /* namespace pcols */
