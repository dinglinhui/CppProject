/*
 * GMDevice.cpp
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#include "GMDevice.h"
#include <cassert>

namespace pcols {

CGMDevice::CGMDevice(void) :
		m_dwSends(0),
		m_dwRecvs(0),
		m_wCacheSize(0),
		m_pCache(nullptr),
		m_pSerialport(nullptr) {
}

CGMDevice::~CGMDevice(void) {
	if (m_pCache != nullptr) {
		delete[] m_pCache;
		m_pCache = nullptr;
	}
}

BOOL CGMDevice::Open(OSComPort *pPort, WORD wCacheSize) {
	assert(pPort != nullptr);
	assert(pPort->IsOpen());
	assert(wCacheSize > 0);

	if (m_pCache != nullptr)
		return false;

	m_pCache = new BYTE[wCacheSize];
	if (m_pCache == nullptr)
		return false;

	m_wCacheSize = wCacheSize;
	m_pSerialport = pPort;
	return true;
}

BOOL CGMDevice::IsOpen(void) {
	if ((m_pSerialport != nullptr) && (m_pCache != nullptr)) {
		return m_pSerialport->IsOpen();
	}
	return false;
}

void CGMDevice::GetComDcb(ComDcb *pDcb) {
	assert(pDcb != nullptr);
	assert(m_pSerialport != nullptr);
	ComDcb *pNow = m_pSerialport->GetCommDCB();
	assert(pNow != nullptr);
	memcpy(pDcb, pNow, sizeof(ComDcb));
}

BOOL CGMDevice::SetComDcb(ComDcb *pDcb) {
	assert(pDcb != nullptr);
	assert(m_pSerialport != nullptr);

	BOOL bRet = false;
	ComDcb *pNow = m_pSerialport->GetCommDCB();
	if (memcmp(pNow, pDcb, sizeof(ComDcb)) != 0) {
		bRet = m_pSerialport->SetCommDCB(pDcb);
	}
	return bRet;
}

void CGMDevice::SaveComDcb(void) {
	assert(m_pSerialport != nullptr);
	ComDcb *pDcb = m_pSerialport->GetCommDCB();
	assert(pDcb != nullptr);
	memcpy(&m_hDCB, pDcb, sizeof(ComDcb));
}

void CGMDevice::RestoreComDcb(void) {
	SetComDcb(&m_hDCB);
}

void CGMDevice::ClearCounter(void) {
	m_dwSends = 0;
	m_dwRecvs = 0;
}

DWORD CGMDevice::GetSendCount(void) {
	return m_dwSends;
}

DWORD CGMDevice::GetRecvCount(void) {
	return m_dwRecvs;
}

int CGMDevice::GetTimesOfSent(int nSize, ComDcb *pDcb) {
	assert(pDcb != nullptr);

	// start(1 bit) + parity(1 bit)
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

	int nTimes = (int) ((nSize * nBits * 1000) / pDcb->nBaud);
	nTimes = nTimes * 1.5;
	return nTimes;
}

int CGMDevice::SendFrame(CGMPoint *pObj, Command *pCMD) {
	assert(pObj != nullptr);
	assert(pCMD != nullptr);
	assert(IsOpen());

	int nDataLen = pObj->HandleTx(pCMD, m_pCache, m_wCacheSize);
	if (nDataLen == 0) {
		// 2006.12.8
		// i want send nothing, but wait respond from peer.
		return 0;
	}

	if (nDataLen > 0) {
		int nSendLen = m_pSerialport->Write(m_pCache, nDataLen);
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
		// because the return value may error
		if (nSendLen > 0) {
			++m_dwSends;
			return nSendLen;
		}
	}

	return -1;
}

int CGMDevice::SendAndWaitRespond(CGMPoint *pObj, Command *pCMD) {
	assert(pObj != nullptr);
	assert(pCMD != nullptr);

	// 2006.11.18
	// because some command of protocol must send by special communication
	// parameters, so add the interface, protocol give the parms by command
	ComDcb hDCB;
	GetComDcb(&hDCB);
	if (pObj->GetSendComDcb(pCMD, &hDCB) == 0) {
		SetComDcb(&hDCB);
	}

	int nSendLen = SendFrame(pObj, pCMD);
	if (nSendLen < 0)
		return -3;

	int nWaitTimes = GetTimesOfSent(nSendLen, &hDCB);

	// 2006.12.5
	// if a command send no respond, we also wait send complete
	// then continue
	if (pCMD->m_nAck == 0) {
		std::this_thread::sleep_for(std::chrono::milliseconds(nWaitTimes));
		return 0;
	} else {
		// 2006.11.18
		// after send some command, must dynamic change communication parms
		// to receive respond stream, the protocol give the parms by command
		if (pObj->GetRecvComDcb(pCMD, &hDCB) == 0) {
			std::this_thread::sleep_for(std::chrono::milliseconds(nWaitTimes));
			SetComDcb(&hDCB);
		}

		int nRet = WaitMeterData(pObj, pCMD);
		if (nRet == 0) {
			++m_dwRecvs;

			// 外扩模块抄表加入延时
			if ((28 == pObj->m_nPort) || (29 == pObj->m_nPort))
				std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		}

		return nRet;
	}

	return -4;
}

int CGMDevice::WaitMeterData(CGMPoint *pObj, Command *pCMD) {
	assert(pObj != nullptr);
	assert(pCMD != nullptr);
	assert(IsOpen());

	memset(m_pCache, 0, m_wCacheSize);

	// 2007.2.28
	// temp buffer length
	static const WORD BFS = 256;

	BYTE lpTmp[BFS] = { 0 };
	DWORD dwTime = 0;	//OSTimeGet();
	DWORD dwExpire = 0;
	WORD nLen = 0;
	WORD nSize = 0;

	int nTranTime = pObj->GetTranTimeOut(pCMD);
	int nRecvTime = pObj->GetRecvTimeOut(pCMD);

	int nWaitTime = nTranTime;
	ComEvent event = COM_UNKNOW;

	do {
		event = m_pSerialport->WaitCommEvent(nWaitTime);
		if (event == COM_RX) {
			do {
				memset(lpTmp, 0, BFS);
				nLen = m_pSerialport->Read(lpTmp, BFS);
				if ((nLen > 0) && (nSize + nLen) <= m_wCacheSize) {
					//	DUMPMSG1(lpTmp, nLen);
					memcpy(&m_pCache[nSize], lpTmp, nLen);
					nSize += nLen;
				}
			} while (nLen > 0);

			nWaitTime = nRecvTime;
			dwTime = 0;	//OSTimeGet();
		} else if (event != COM_TIMEOUT) {
			dwExpire = (DWORD) (/*OSTimeGet() - */dwTime);
			if (dwExpire >= nTranTime) {
				break;
			}
		}
	} while (event != COM_TIMEOUT);

	return pObj->HandleRx(pCMD, m_pCache, nSize);
}

} /* namespace pcols */
