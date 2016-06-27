/*
 * CircleBuff.h
 *
 *  Created on: 2013-6-19
 *      Author: dinglinhui
 */

#ifndef CIRCLEBUFF_H_
#define CIRCLEBUFF_H_
#include <string>
#include "OSDefs.h"

namespace utils {
class CCircleBuff {
public:
	CCircleBuff(int MaxCount, int SizeOfByte = 1);
	virtual ~CCircleBuff();
	//
	int Read(void *buff, int length);
	int Write(const void *buff, const int length);
	//
	void Flush();
	//
	int GetCount() const;
	int GetMaxCount() const;
	//
	bool isOK(void) const;

protected:
	bool Delete();

private:
	BYTE * m_pBuff;
	BYTE * m_pBuffEnd;
	BYTE * m_pOut;
	BYTE * m_pIn;
	int m_nCount;
	const int m_nMaxCount;
	const int m_nSizeOfByte;
	bool m_bOk;
};

}

#endif /* CCIRCLEBUFF_H_ */
