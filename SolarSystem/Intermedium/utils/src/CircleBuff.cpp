/*
 * CCircleBuff.cpp
 *
 *  Created on: 2013-6-19
 *      Author: dinglinhui
 */

#include "CircleBuff.h"

namespace utils {

CCircleBuff::CCircleBuff(int MaxCount, int SizeOfByte) :
		m_pBuff(nullptr), m_pBuffEnd(nullptr), m_pOut(nullptr), m_pIn(nullptr), m_nCount(0), m_nMaxCount(MaxCount), m_nSizeOfByte(
				SizeOfByte), m_bOk(false) {
	m_pBuff = new BYTE[MaxCount * SizeOfByte];
	if (m_pBuff != nullptr) {
		m_nCount = 0;
		m_pOut = m_pBuff;
		m_pIn = m_pBuff;
		m_pBuffEnd = m_pBuff + (MaxCount * SizeOfByte);
		m_bOk = true;
	}
}

CCircleBuff::~CCircleBuff() {
	Delete();
	Flush();
}

int CCircleBuff::Read(void *pBuff, int length) {
	int count = length;
	BYTE *buff = (BYTE *) pBuff;

	if (!m_bOk)
		return -1;

	if (buff == nullptr)
		return -1;

	if ((length > m_nCount) || (length == 0))
		count = m_nCount;

	if (count > 0) {
		int bytes1, bytes2;

		bytes1 = count * m_nSizeOfByte;
		if ((m_pOut + bytes1) >= m_pBuffEnd) {
			bytes2 = m_pBuffEnd - m_pOut;
			memcpy(buff, m_pOut, bytes2);

			buff += bytes2;

			memcpy(buff, m_pBuff, bytes1 - bytes2);
			//
			m_pOut = m_pBuff + (bytes1 - bytes2);
		} else {
			memcpy(buff, m_pOut, bytes1);
			//
			m_pOut += bytes1;
		}

		m_nCount -= count;
	}

	return count;
}

int CCircleBuff::Write(const void *pBuff, const int length) {
	int count = length;
	const BYTE* buff = (BYTE *) pBuff;

	if (!m_bOk)
		return -1;

	if (pBuff == nullptr)
		return -1;

	if (count <= 0)
		return -1;

	if (count > (m_nMaxCount - m_nCount))
		count = m_nMaxCount - m_nCount;

	if (count > 0) {
		int bytes1, bytes2;

		bytes1 = count * m_nSizeOfByte;

		if ((m_pIn + bytes1) >= m_pBuffEnd) {
			bytes2 = m_pBuffEnd - m_pIn;
			memcpy(m_pIn, buff, bytes2);

			buff += bytes2;

			memcpy(m_pBuff, buff, bytes1 - bytes2);
			//
			m_pIn = m_pBuff + (bytes1 - bytes2);
		} else {
			memcpy(m_pIn, buff, bytes1);
			//
			m_pIn += bytes1;
		}

		m_nCount += count;
	}

	return count;
}

void CCircleBuff::Flush() {
	m_nCount = 0;
	m_pOut = m_pBuff;
	m_pIn = m_pBuff;
}

bool CCircleBuff::isOK(void) const {
	return m_bOk;
}

int CCircleBuff::GetCount() const {
	return m_nCount;
}

int CCircleBuff::GetMaxCount() const {
	return m_nMaxCount;
}

bool CCircleBuff::Delete() {
	if (m_pBuff != nullptr) {
		delete[] m_pBuff;
		m_pBuff = nullptr;
	}
	return true;
}
}
