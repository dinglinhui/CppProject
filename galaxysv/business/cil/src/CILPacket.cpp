/*
 * CILPacket.cpp
 *
 *  Created on: Jun 13, 2015
 *      Author: dinglinhui
 */

#include "../../cil/inc/CILPacket.h"

#include <cassert>

namespace syscil {
CILPacket::CILPacket(PCOL_TYPE type) :
		m_nProtocol(type),
		m_lpBuf(nullptr),
		m_nBufSize(0),
		m_nDataLen(0),
		m_bDelete(false),
		m_pUserPtr(nullptr) {
}

CILPacket::CILPacket(PCOL_TYPE type, int nSize) :
		m_nProtocol(type),
		m_lpBuf(nullptr),
		m_nBufSize(nSize),
		m_nDataLen(0),
		m_bDelete(true),
		m_pUserPtr(nullptr) {
	AllocBuffer(nSize);
}

CILPacket::CILPacket(PCOL_TYPE type, BYTE *lpBuf, int nSize) :
		m_nProtocol(type),
		m_lpBuf(lpBuf),
		m_nBufSize(nSize),
		m_nDataLen(0),
		m_bDelete(false),
		m_pUserPtr(nullptr) {
}

CILPacket::~CILPacket(void) {
	FreeBuffer();
}

bool CILPacket::AllocBuffer(int nSize) {
//	m_lpBuf = (BYTE*) mempool_alloc(nSize);
	m_lpBuf = nullptr;
	if (m_lpBuf != nullptr) {
		m_nBufSize = nSize;
		m_bDelete = true;
		return true;
	}
	return false;
}

void CILPacket::FreeBuffer(void) {
	if (m_bDelete) {
		if (m_lpBuf != nullptr) {
//			mempool_free(m_lpBuf, m_nBufSize);
			delete m_lpBuf;
		}
		m_lpBuf = nullptr;
		m_nBufSize = 0;
	}
}

int CILPacket::Append(BYTE *lpBuf, int nSize) {
	if (m_lpBuf == nullptr) {
		AllocBuffer(nSize);
		if (m_lpBuf == nullptr) {
			return -1;
		}
	}

	int nSpace = m_nBufSize - m_nDataLen;
	if (nSpace < nSize)
		return -1;

	memcpy(&m_lpBuf[m_nDataLen], lpBuf, nSize);
	m_nDataLen += nSize;
	return 0;
}

int CILPacket::Offset(int nSize) {
	if (m_lpBuf != nullptr) {
		if ((m_nDataLen + nSize) <= m_nBufSize) {
			m_nDataLen += nSize;
			return 0;
		}
	}
	return -1;
}

int CILPacket::Remove(int nSize) {
	if (m_lpBuf == nullptr)
		return -1;

	if (nSize > m_nDataLen) {
		Flush();
		return m_nDataLen;
	}

	memmove(m_lpBuf, &m_lpBuf[nSize], nSize);
	m_nDataLen -= nSize;
	return nSize;
}

void CILPacket::Flush(void) {
	m_nDataLen = 0;
}

void CILPacket::SetUserPtr(void *ptr) {
	m_pUserPtr = ptr;
}

void* CILPacket::GetUserPtr(void) {
	return m_pUserPtr;
}

void CILPacket::GetUserPtr(void**& pptr) {
	pptr = &m_pUserPtr;
}

PCOL_TYPE CILPacket::GetPcolType(void) const {
	return m_nProtocol;
}

BYTE* CILPacket::GetFrame(void) const {
	return m_lpBuf;
}

int CILPacket::GetData(BYTE *&ptr) {
	ptr = m_lpBuf;
	return m_nDataLen;
}

PCMD_TYPE CILPacket::GetPcmdType(void) {
	return PCMD_BROADCAST;
}

int CILPacket::GetSize(void) const {
	return m_nBufSize;
}

int CILPacket::GetLength(void) const {
	return m_nDataLen;
}

int CILPacket::Create(BYTE *lpBuf, int nSize) {
	Flush();
	if (Append(lpBuf, nSize) == 0) {
		return nSize;
	}
	return 0;
}

int CILPacket::IsValid(void) {
	if (m_lpBuf != nullptr) {
		return 0;
	}
	return -1;
}

void* CILPacket::operator new(size_t nSize) {
//	void *ptr = mempool_alloc(nSize);
	void *ptr = nullptr;
	assert(ptr != nullptr);
	return ptr;
}

void CILPacket::operator delete(void *ptr) {
	assert(ptr != nullptr);
//	mempool_free(ptr);
	delete[] (BYTE *) ptr;
}

CILPacket* CILPacket::Clone(void) {
	CILPacket *pk = new CILPacket(m_nProtocol, m_nBufSize);
	if (pk != nullptr) {
		pk->Append(m_lpBuf, m_nDataLen);

	}
	return pk;
}

CILPacket* CILPacket::Duplicate(void) {
	CILPacket *pk = new CILPacket(m_nProtocol, m_lpBuf, m_nBufSize);
	if (pk != nullptr) {
		pk->Offset(m_nDataLen);
	}
	return pk;
}

} /* namespace syscil */
