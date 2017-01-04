/*
 * CILRequest.h
 *
 *  Created on: Jun 13, 2015
 *      Author: dinglinhui
 */

#ifndef CILREQUEST_H_
#define CILREQUEST_H_

#include "OSDefs.h"
#include "CILDevice.h"
#include "CILPacket.h"
#include "CILService.h"
#include "OSMessageBase.h"

namespace syscil {
using namespace osext;

class CILPacket;
class CILDevice;
class CILRequest {
public:
	CILRequest(CILDevice *pVxd, CILPacket *pk = nullptr, OSMessageBase *phdr = nullptr);
	virtual ~CILRequest() {
	}

	virtual void Run(void);

	CILRequest* m_pNext;

private:
	bool IsAskOf(CILPacket *pk);
	bool IsAckOf(CILPacket *pkSend, CILPacket *pkRecv);

	CILDevice* m_pDevice;
	CILPacket* m_pPacket;
	OSMessageBase* m_pSender;
};

} /* namespace syscil */

#endif /* CILREQUEST_H_ */
