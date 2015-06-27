/*
 * GMPoint.cpp
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#include "GMPoint.h"

#include "GMProtocol.h"
#include "GMScheduler.h"

namespace pcols {
///////////////////////////////////////////////////////////////////////////////
// CGMPoint
///////////////////////////////////////////////////////////////////////////////

CGMPoint::CGMPoint(CGMScheduler *parent) :
		m_TN(-1), m_nActive(0), m_FailTimes(0), m_nPointType(Mul_Point), m_nPort(
				-1), m_nBaud(-1), m_nDatabits(-1), m_nStopbits(-1), m_nParity(
				-1), m_pNext( nullptr), m_pProtocol( nullptr), m_pScheduler(parent), m_nMPTimeEnable(
				3) // 默认时间有效
{
	memset(m_host, 0, 6);
	memset(m_user, 0, 6);
	memset(m_password, 0, 6);
}

CGMPoint::~CGMPoint(void) {
}

int CGMPoint::RequestHook(Command *) {
	return -1;
}

int CGMPoint::GetBaseComDcb(ComDcb *pDcb) {

	if (m_pProtocol != nullptr) {
		m_pProtocol->GetDefaultComDcb(pDcb);
	}

//	ConvertBaud(pDcb->nBaud, m_nBaud);
//	ConvertDatabit(pDcb->databit, m_nDatabits);
//	ConvertParity(pDcb->parity, m_nParity);
//	ConvertStopbit(pDcb->stopbit, m_nStopbits);

	return 0;
}

int CGMPoint::GetSendComDcb(Command *pCMD, ComDcb *pDcb) {
	int nRet = -1;
	if (m_pProtocol != nullptr) {
		nRet = m_pProtocol->GetSendComDcb(pCMD, pDcb);
	}

	if (0 != nRet) {
		nRet = GetBaseComDcb(pDcb);
	}

	return nRet;
}

int CGMPoint::GetRecvComDcb(Command *pCMD, ComDcb *pDcb) {
	int nRet = -1;
	if (m_pProtocol != nullptr) {
		nRet = m_pProtocol->GetRecvComDcb(pCMD, pDcb);
	}

	if (0 != nRet) {
		nRet = GetBaseComDcb(pDcb);
	}

	return nRet;
}

int CGMPoint::GetTranTimeOut(Command *pCMD) {
	if ((28 == m_nPort) || (29 == m_nPort))
		return 100 * 100; // 100s  10000tick

	if (m_pProtocol != nullptr) {
		return m_pProtocol->GetTranTimeOut(pCMD);
	}
	return MP_TRANTIMEOUT;
}

int CGMPoint::GetRecvTimeOut(Command *pCMD) {
	if (m_pProtocol != nullptr) {
		return m_pProtocol->GetRecvTimeOut(pCMD);
	}
	return MP_RECVTIMEOUT;
}

int CGMPoint::MakeCommands(void *parm, Command *&pCMDs) {
	if (m_pProtocol != nullptr) {
		// 预抄表命令
		if (((int) *(int *)parm) == 4) {
			parm = (void*) 1;
		}
		// end

		return m_pProtocol->GetCommands(parm, pCMDs, m_nPointType);
	}
	return -1;
}

int CGMPoint::HandleTx(Command *pCMD, BYTE *lpBuf, int nBufSize) {
	if (m_pProtocol != nullptr) {
		return m_pProtocol->HandleTx(this, pCMD, lpBuf, nBufSize);
	}
	return -1;
}

int CGMPoint::HandleRx(Command *pCMD, BYTE *lpBuf, int nBufSize) {
	if (m_pProtocol != nullptr) {
		return m_pProtocol->HandleRx(this, pCMD, lpBuf, nBufSize);
	}
	return -1;
}

// add by jsh
// 2009-3-30
int CGMPoint::GetRetryCount(void) {
	if (m_pProtocol != nullptr) {
		return m_pProtocol->GetRetryCount();
	}

	return 0;
}

} /* namespace pcols */
