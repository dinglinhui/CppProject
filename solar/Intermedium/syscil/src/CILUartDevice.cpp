/*
 * CILUartDevice.cpp
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#include <cassert>
#include <CILUartDevice.h>

namespace syscil {

CILUartDevice::CILUartDevice(BYTE nIdent, int nCacheSize, DWORD tmTran, DWORD tmRecv) :
		CILDevice(nIdent, nCacheSize),
		m_pPort( nullptr),
		m_tmTran(tmTran),
		m_tmRecv(tmRecv) {
}

CILUartDevice::~CILUartDevice(void) {
}

void CILUartDevice::Init(OSComPort *ptr) {
	assert(ptr != nullptr);
	m_pPort = ptr;
}

DWORD CILUartDevice::GetTranTimeout(void) const {
	return m_tmTran;
}

DWORD CILUartDevice::GetRecvTimeout(void) const {
	return m_tmRecv;
}

DWORD CILUartDevice::GetTimesOfSent(int nSize) {
	if (m_pPort == nullptr)
		return 0;

	ComDcb *pDcb = m_pPort->GetCommDCB();
	assert(pDcb != nullptr);

	// 1 bit of start, 1 bit of parity
	BYTE nBits = 2;
	if (pDcb->databit == DB_5)
		nBits += 5;
	else if (pDcb->databit == DB_6)
		nBits += 6;
	else if (pDcb->databit == DB_7)
		nBits += 7;
	else
		nBits += 8;

	if (pDcb->stopbit == CS_ONE)
		nBits += 1;
	else
		nBits += 2;

	if (pDcb->nBaud == 0) {
		pDcb->nBaud = CB_9600;
	}
	return (DWORD) ((nSize * nBits * 1000) / pDcb->nBaud);
}

int CILUartDevice::Write(BYTE* lpBuf, int nBufSize) {
	if (m_pPort == nullptr)
		return -1;
	if (!m_pPort->IsOpen())
		return -1;

	int nRet = m_pPort->Write(lpBuf, nBufSize);
	if (nRet > 0) {
		DWORD dwTms = GetTimesOfSent(nBufSize);
		std::this_thread::sleep_for(std::chrono::milliseconds(dwTms));
	}
	return nRet;
}

int CILUartDevice::Read(BYTE* lpBuf, int nSize, DWORD dwTimeout) {
	if (m_pPort == nullptr)
		return -1;
	if (!m_pPort->IsOpen())
		return -1;

	ComEvent event = COM_UNKNOW;
	DWORD tmOut = (dwTimeout == 0) ? m_tmTran : dwTimeout;
	DWORD tmWait = tmOut;
	DWORD tmOver = 0; //OSTimeGet();
	DWORD dwExpire = 0;

	int nSpace = nSize;
	int ndtLen = 0;
	int nRecv = 0;

	do {
		event = m_pPort->WaitCommEvent(tmWait);
		if (m_pPort->GetRxDataLen() > 0) {
			nRecv = m_pPort->Read(&lpBuf[ndtLen], nSpace);
			nSpace -= nRecv;
			ndtLen += nRecv;
			if (nSpace <= 0) {
				break;
			}
			tmWait = m_tmRecv;
		} else if ((event != COM_TIMEOUT) && (event != COM_RX)) {
			dwExpire = (DWORD) (/*OSTimeGet() - */tmOver);
			if (dwExpire >= tmOut) {
				break;
			}
		}
	} while (event != COM_TIMEOUT);

	return ndtLen;
}

} /* namespace syscil */
