/*
 * CILDevice.cpp
 *
 *  Created on: Jun 13, 2015
 *      Author: dinglinhui
 */

#include "CILDevice.h"
#include "CILPacketFactory.h"

namespace syscil {
CILDevice::CILDevice() :
		m_pNext(nullptr), m_nIdent(0), m_nProtocol(PCOL_STREAM), m_bUsable(true), m_nCacheSize(0), m_nCacheLen(0), m_lpCache(
				nullptr), m_isOk(false), m_plstSend(nullptr), m_pService(nullptr) {
}

CILDevice::~CILDevice() {
}

int CILDevice::Send(CILPacket *pk, OSMessageBase *phdr) {
	CILRequest *ptr = new CILRequest(this, pk, phdr);
	if (ptr != nullptr) {
//		OSCriticalSection lock;
		CILRequest **ppPrev = &m_plstSend;
		while (*ppPrev != nullptr) {
			ppPrev = &(*ppPrev)->m_pNext;
		}
		ptr->m_pNext = *ppPrev;
		*ppPrev = ptr;
		return 0;
	}
	return -1;
}

int CILDevice::Peek(CILRequest *& pk) {
	if (m_plstSend == nullptr) {
		pk = new CILRequest(this);
		return (pk != nullptr) ? 0 : -1;
	}

//	OSCriticalSection lock;
	pk = m_plstSend;
	m_plstSend = m_plstSend->m_pNext;
	return 0;
}

int CILDevice::SendPacket(CILPacket *pkSend) {
	if (!Usable())
		return -1;

	CILPacket *pkRt = pkSend;
	if (pkRt != nullptr) {
		BYTE *lpData = pkRt->GetFrame();
		int nDataLen = pkRt->GetLength();
		int nSendLen = Write(lpData, nDataLen);

		if (pkRt != pkSend) {
			delete pkRt;
		}

		return nSendLen;
	}

	return -2;
}

int CILDevice::RecvPacket(CILPacket *& pkRecv, DWORD dwTimeout) {
	if (!Usable())
		return -1;

	if (m_nCacheLen == 0) {
		memset(m_lpCache, 0, m_nCacheSize);
		m_nCacheLen = Read(m_lpCache, m_nCacheSize, dwTimeout);
	}

	if (m_nCacheLen <= 0)
		return -2;

	CILPacket *pkOrg = CILPacketFactory::Create(m_nProtocol);
	if (pkOrg == nullptr)
		return -3;

	int nLen = pkOrg->Create(m_lpCache, m_nCacheLen);
	if (nLen <= 0) {
		m_nCacheLen = 0;
		delete pkOrg, pkOrg = nullptr;
		return 1;
	}

	int nSpan = m_nCacheLen - nLen;
	if (nSpan > 0) {
		memmove(m_lpCache, &m_lpCache[nLen], nSpan);
	}
	m_nCacheLen = nSpan;

	pkRecv = pkOrg;

	return 0;
}

int CILDevice::Write(BYTE*, int) {
	return 0;
}

int CILDevice::Read(BYTE*, int, DWORD) {
	return 0;
}
} /* namespace syscil */
