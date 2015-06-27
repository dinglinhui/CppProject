/*
 * CILDevice.h
 *
 *  Created on: Jun 13, 2015
 *      Author: dinglinhui
 */

#ifndef CILDEVICE_H_
#define CILDEVICE_H_

#include "OSDefs.h"
#include "CILPacket.h"
#include "CILService.h"
#include "CILRequest.h"
#include "OSMessageBase.h"

namespace syscil {
using namespace osext;

class CILPacket;
class CILService;
class CILRequest;
class CILDevice {
public:
	CILDevice(BYTE nIdent, int nCacheSize);
	virtual ~CILDevice();

	void SetService(CILService *pService) {
		m_pService = pService;
	}
	CILService* GetService(void) {
		return m_pService;
	}

	BYTE GetIdent(void) const {
		return m_nIdent;
	}

	void Enable(bool bEnable = true) {
		m_bUsable = bEnable;
	}
	bool Usable(void) const {
		if (m_bUsable) {
			return (m_lpCache != nullptr);
		}
		return false;
	}

	int Send(CILPacket *pk, OSMessageBase *phdr);
	int Peek(CILRequest *& pk);

	int SendPacket(CILPacket *pk);

	CILDevice *m_pNext;

protected:
	int RecvPacket(CILPacket *&pk, DWORD dwTimeOut = 0);

	virtual int Write(BYTE *lpBuf, int nSize);
	virtual int Read(BYTE *lpBuf, int nSize, DWORD dwTimeout);

private:
	CILDevice(const CILDevice &);
	CILDevice& operator=(const CILDevice &);

	friend class CILRequest;

	const BYTE m_nIdent;
	PCOL_TYPE m_nProtocol;
	bool m_bUsable;
	int m_nCacheSize;
	int m_nCacheLen;
	BYTE* m_lpCache;
	bool m_isOk;

	CILRequest* m_plstSend;
	CILService* m_pService;
};

} /* namespace syscil */

#endif /* CILDEVICE_H_ */
