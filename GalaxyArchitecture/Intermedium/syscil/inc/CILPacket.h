/*
 * CILPacket.h
 *
 *  Created on: Jun 13, 2015
 *      Author: dinglinhui
 */

#ifndef CILPACKET_H_
#define CILPACKET_H_

#include <iostream>
#include "OSDefs.h"

namespace syscil {
class CILPacket {
public:
	CILPacket(PCOL_TYPE type);
	CILPacket(PCOL_TYPE type, int nSize);
	CILPacket(PCOL_TYPE type, BYTE *lpBuf, int nSize);
	virtual ~CILPacket(void);

	int Append(BYTE *lpBuf, int nSize);
	int Offset(int nSize);
	int Remove(int nSize);
	void Flush(void);

	PCOL_TYPE GetPcolType(void) const;
	BYTE* GetFrame(void) const;
	int GetSize(void) const;
	int GetLength(void) const;

	void SetUserPtr(void *ptr);
	void* GetUserPtr(void);
	void GetUserPtr(void**& pptr);

	virtual int Create(BYTE *lpBuf, int nSize);
	virtual int IsValid(void);
	virtual int GetData(BYTE *& ptr);

	virtual PCMD_TYPE GetPcmdType(void);
	virtual CILPacket* Clone(void);
	virtual CILPacket* Duplicate(void);

	static void* operator new(size_t nSize);
	static void operator delete(void *ptr);

private:
	bool AllocBuffer(int nSize);
	void FreeBuffer(void);

	PCOL_TYPE m_nProtocol;
	BYTE* m_lpBuf;
	int m_nBufSize;
	int m_nDataLen;
	bool m_bDelete;
	void* m_pUserPtr;
};

} /* namespace syscil */

#endif /* CILPACKET_H_ */
