/*
 * GMScheduler.cpp
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#include <cstdlib>
#include <cassert>
#include <ctypes.h>
#include "GMScheduler.h"

namespace pcols {

CGMScheduler::CGMScheduler(int nPrio, int nStackSize, int nQueueSize) :
		OSThreadEx(nPrio, nStackSize, nQueueSize),
		m_nPort(0),
		m_plstGMP(nullptr),
		m_pGMDevice(nullptr),
		m_plstTask(nullptr)/*, m_hLock(1, false)*/{
}

CGMScheduler::~CGMScheduler(void) {
}

int CGMScheduler::GetPort(void) const {
	return m_nPort;
}

int CGMScheduler::Create(int nPort, OSComPort *port, WORD wCacheSize) {
	assert(port != nullptr);
	assert(port->IsOpen());

	if (m_pGMDevice == nullptr) {
		m_pGMDevice = new CGMDevice();
		if (m_pGMDevice != nullptr) {
			if (m_pGMDevice->Open(port, wCacheSize)) {
				m_nPort = nPort;
				return 0;
			}
		}
	}
	return -1;
}

void CGMScheduler::ResetComDcb(CGMPoint *pObj) {
	assert(pObj != nullptr);
	ComDcb hDCB;
	m_pGMDevice->GetComDcb(&hDCB);
	pObj->GetBaseComDcb(&hDCB);
	m_pGMDevice->SetComDcb(&hDCB);
}

CGMPoint* CGMScheduler::Find(WORD tn) {
//	OSCriticalSection lock;
	CGMPoint *mp = m_plstGMP;
	while (mp != nullptr) {
		if (mp->m_TN == tn) {
			return mp;
		}
		mp = mp->m_pNext;
	}
	return nullptr;
}
/*
 CGMPoint* CGMScheduler::Find( WORD tn, int nPort )
 {
 OSCriticalSection lock;
 CGMPoint *mp = m_plstGMP;
 while( mp != nullptr )
 {
 if( (mp->m_TN == tn) && (nPort == mp->m_nPort) )
 {
 return mp;
 }
 mp = mp->m_pNext;
 }
 return nullptr;
 }*/

CGMPoint* CGMScheduler::FindFirst(void) {
//	OSCriticalSection lock;
	CGMPoint *pObj = m_plstGMP;
	while (pObj != nullptr) {
//		if( pObj->m_nPort == m_nPort )
		{
			return pObj;
		}
		pObj = pObj->m_pNext;
	};
	return nullptr;
}

CGMPoint* CGMScheduler::FindNext(CGMPoint *pObj) {
//	OSCriticalSection lock;
	assert(pObj != nullptr);
	CGMPoint *pNext = pObj;
	do {
		pNext = pNext->m_pNext;
		if (pNext != nullptr) // && (pNext->m_nPort == m_nPort) )
				{
			return pNext;
		}
	} while (pNext != nullptr);

	return nullptr;
}

CGMPoint* CGMScheduler::FindFirstActive(void) {
//	OSCriticalSection lock;
	CGMPoint *pObj = m_plstGMP;
	while (pObj != nullptr) {
		if (/* (pObj->m_nPort == m_nPort) &&*/
		(pObj->m_nActive == 1)) {
			return pObj;
		}
		pObj = pObj->m_pNext;
	};
	return nullptr;
}

CGMPoint* CGMScheduler::FindNextActive(CGMPoint *pObj) {
	if (pObj == nullptr) {
		assert(false);
		return nullptr;
	}

//	OSCriticalSection lock;
	assert(pObj != nullptr);
	CGMPoint *pNext = pObj;
	do {
		pNext = pNext->m_pNext;
		if ((pNext != nullptr) &&
		/*(pNext->m_nPort == m_nPort) &&*/
		(pNext->m_nActive == 1)) {
			return pNext;
		}
	} while (pNext != nullptr);

	return nullptr;
}

void CGMScheduler::RemoveHead(void) {
//	OSCriticalSection lock;
	CGMTask *pTask = m_plstTask;
	if (pTask != nullptr) {
		m_plstTask = pTask->m_pNext;
		delete pTask;
		pTask = nullptr;
	}
}

void CGMScheduler::AddToTail(CGMTask *pTask) {
	assert(pTask != nullptr);
//	OSCriticalSection lock;

	CGMTask **ppNxt = (CGMTask**) &m_plstTask;
	while (*ppNxt != nullptr) {
		ppNxt = &(*ppNxt)->m_pNext;
	}

	pTask->m_pNext = *ppNxt;
	*ppNxt = pTask;
}

void CGMScheduler::AddGMPoint(CGMPoint *pMP) {
//	OSCriticalSection lock;

	CGMPoint **ppNxt = (CGMPoint**) &m_plstGMP;
	while (*ppNxt != nullptr) {
		ppNxt = &(*ppNxt)->m_pNext;
	}

	*ppNxt = pMP;
}

void CGMScheduler::DelGMPoint(CGMPoint *pMP) {
//	OSCriticalSection lock;

	CGMPoint ** ppPrev = &m_plstGMP;

	while (*ppPrev != nullptr && *ppPrev != pMP) {
		ppPrev = &(*ppPrev)->m_pNext;
	}

	if (*ppPrev != nullptr) {
		*ppPrev = pMP->m_pNext;
	}
}

void CGMScheduler::OnSuccess(CGMTask *pTask, WORD TN, GMRET ret) {
	assert(pTask != nullptr);
	if (nullptr != pTask) {
		if (nullptr != (OSMessageBase*) pTask->m_pSrce) {
			PostMessage((OSMessageBase*) pTask->m_pSrce, MSGType::GM_SUCCESS, MAKELONG(TN, ret), (DWORD) pTask->m_pBody);
			return;
		}
	}
}

void CGMScheduler::OnFinish(CGMTask *pTask, WORD TN, GMRET ret) {
	assert(pTask != nullptr);
	if (nullptr != pTask) {
		if (nullptr != (OSMessageBase*) pTask->m_pSrce) {
			PostMessage((OSMessageBase*) pTask->m_pSrce, MSGType::GM_FINISH, MAKELONG(TN, ret), (DWORD) pTask->m_pBody);
			return;
		}
	}
}

OSRet CGMScheduler::OSInitHook(void) {
	if (m_pGMDevice == nullptr) {
		assert(false);
		return OSRet::ERROR;
	}

//	if (!m_hLock.Create()) {
//		assert (false);
//		return -1;
//	}

	return OSRet::OK;
}

int CGMScheduler::GetRetryCount(CGMPoint* pMP) {
	assert(pMP != nullptr);

	return pMP->GetRetryCount();
}

void CGMScheduler::MeterReset(void) {
	while (m_plstTask != nullptr) {
		RemoveHead();
	}
}

int CGMScheduler::RequestData(CGMPoint *pObj, Command *pCMD) {
	assert(pObj != nullptr);
	assert(pCMD != nullptr);
	assert(m_pGMDevice != nullptr);

	int nRet = -1;

//	if (m_hLock.Pend() == OS_NO_ERR) {
//		nRet = pObj->RequestHook(pCMD);
//		if (nRet < 0) {
//			BYTE nRetry = GetRetryCount(pObj);
//
//			do {
//				nRet = m_pGMDevice->SendAndWaitRespond(pObj, pCMD);
//
//				if (nRet == 0) {
//					break;
//				} else {
//					if ((--nRetry) > 0) {
//						OSSleep(1000);
//					}
//				}
//			} while (nRetry > 0);
//		}
//
//		m_hLock.Post();
//	}

	return nRet;
}

int CGMScheduler::OnHandleMessage(OSMessage *msg) {
	assert(msg != nullptr);
	int nRet = -1;

	switch (msg->m_nCmd) {
	case MSGType::GM_RUN: {
		WORD tn = (WORD) msg->m_wParam;
		void* ptr = (void*) msg->m_lParam;/*-只取后一个字节，前一个字节区分是抄表间隔抄表还是及时抄4/10/2009 1:14PMgs-*/

		CGMTask *pTask = new CGMTask;
		if (pTask != nullptr) {
			pTask->m_nTN = tn;
			pTask->m_pBody = ptr;
			pTask->m_pSrce = msg->m_pSource;

			if (msg->m_bAsyn) {
				AddToTail(pTask);
				PostMessage(this, MSGType::GM_STEP, 0, 0);
			} else {
				DWORD dwRet = 0;
				BYTE ret = 0;
				do {
					dwRet = pTask->Execute(this);
					ret = (BYTE) LOWORD(dwRet);
					if ((ret & GM_POINT) == GM_POINT) {
						if ((ret & GM_OK) == GM_OK) {
							nRet = 0;
						}
					}
				} while ((ret != GM_EXCEPTION) && ((ret & GM_COMPLETE) != GM_COMPLETE));

				delete pTask;
			}
		}
	}
		break;

	case MSGType::GM_STEP: {
		CGMTask *pTask = m_plstTask;
		if (pTask != nullptr) {
			DWORD dwRet = pTask->Execute(this);

			WORD tn = HIWORD(dwRet);
			BYTE ret = (BYTE) LOWORD(dwRet);

			// task all items execute complete
			if ((ret & 0xF0) == GM_COMPLETE) {
				OnFinish(pTask, tn, (GMRET) (ret & 0x0F));
				RemoveHead();
				PostMessage(this, MSGType::GM_STEP, 0, 0);
			}

			// task parameters error
			else if (ret == GM_EXCEPTION) {
				OnFinish(pTask, tn, GM_ERROR);
				RemoveHead();
				PostMessage(this, MSGType::GM_STEP, 0, 0);
			}

			// one command execute complete,
			// then gather next command
			else if ((ret & 0xF0) == GM_ITEM) {
				PostMessage(this, MSGType::GM_STEP, 0, 0);
			}

			// one measure point execute complete
			// then gather next measure point
			else if ((ret & 0xF0) == GM_POINT) {
				OnSuccess(pTask, tn, (GMRET) (ret & 0x0F));
				PostMessage(this, MSGType::GM_STEP, 0, 0);
			}

//			OSSleep(30);
		}
	}
		break;

	default:
		nRet = OSThreadEx::OnHandleMessage(msg);
		break;
	};

	return nRet;
}

} /* namespace pcols */
