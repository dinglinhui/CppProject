/*
 * CILRequest.cpp
 *
 *  Created on: Jun 13, 2015
 *      Author: dinglinhui
 */

#include <cassert>
#include "OSDefs.h"
#include "CILRequest.h"

namespace syscil {

CILRequest::CILRequest(CILDevice *pDevice, CILPacket *pPacket, OSMessageBase *pSender) :
		m_pNext(nullptr), m_pDevice(pDevice), m_pPacket(pPacket), m_pSender(pSender) {
}

bool CILRequest::IsAskOf(CILPacket *pk) {
	assert(pk != nullptr);
	int nAsk = 0;

	PCMD_TYPE cmd = pk->GetPcmdType();
	if (cmd == PCMD_REQUESTANY)
		nAsk = 2;
	else if (cmd == PCMD_REQUEST)
		nAsk = 1;
	else
		nAsk = 0;

	return (nAsk > 0);
}

bool CILRequest::IsAckOf(CILPacket *pkSend, CILPacket *pkRecv) {
	assert(pkSend != nullptr);
	assert(pkRecv != nullptr);

	DWORD typeSend = pkSend->GetPcolType();
	DWORD typeRecv = pkRecv->GetPcolType();

	if (typeSend != typeRecv) {
		return false;
	}

	PCMD_TYPE cmdSend = pkSend->GetPcmdType();
	if (cmdSend == PCMD_REQUESTANY) {
		return true;
	} else if (cmdSend == PCMD_REQUEST) {
		PCMD_TYPE cmdRecv = pkRecv->GetPcmdType();
		return (cmdRecv == PCMD_RESPOND);
	}

	return false;
}

void CILRequest::Run(void) {
	BOOL bAsk = false;
	BOOL bAck = false;
	BOOL bDoit = false;
	DWORD tmWait = (m_pPacket == nullptr) ? 1 : 0;

//	if (m_pDevice->Lock(tmWait) == 0)
	{
		// Send packet to peer
		// If packet's command type is 'Request',
		// we will wait respond until recv correct or timeout.
		if (m_pPacket != nullptr) {
			m_pDevice->SendPacket(m_pPacket);
			bAsk = IsAskOf(m_pPacket);
//			DWORD dwDelay = (bAsk) ? 1000 : 500;
//			OSSleep(dwDelay);
		}

		tmWait = (bAsk) ? 0 : 1;

		CILPacket *pPacket = nullptr;
		OSMessageBase *pExt = nullptr;
		OSMessageBase *pEntry = nullptr;

		CILService *pService = m_pDevice->GetService();
		if (pService != nullptr) {
			pEntry = pService->GetPackEntry();
		}

		int nRet = -1;

		do {
			// Query mode or wait respond mode
			// Query 'tmWait' is 20 milliseconds, wait respond 'tmWait' is 3 seconds.
			nRet = m_pDevice->RecvPacket(pPacket, tmWait);
			while (pPacket != nullptr) {
				bAck = false;
				if (bAsk) {
					bAck = IsAckOf(m_pPacket, pPacket);
				}

				// wait respond correct, notify to sender
				// change wait respond mode to query mode
				if (bAck && bAsk && (!bDoit)) {
					bDoit = true;
					pExt = m_pSender;
					tmWait = 1;
				} else {
					// export packet to the only receiver
					pExt = pEntry;
				}

				// notify to receiver
				int nOk = -1;
				if (pExt != nullptr) {
					nOk = pExt->PostMessage(pExt, MSGType::CIL_RECVPACKET, m_pDevice->GetIdent(), (DWORD) pPacket);
				}

				CILPacket *pNext = (CILPacket*) pPacket->GetUserPtr();
				if (nOk != 0) {
					assert(false);
					delete pPacket;
				}
				pPacket = pNext;
			}
		} while ((nRet == 0) || ((nRet > 0) && bAsk));

//		m_pDevice->UnLock();
	}

	// When request and wait respond timeout,
	// we send a nullptr packet notify sender.
	if (bAsk && (!bDoit)) {
		if (m_pSender != nullptr) {
			int nOk = m_pSender->PostMessage(m_pSender, MSGType::CIL_TIMEOUT, m_pDevice->GetIdent(), (DWORD) m_pPacket);
			if (nOk == 0) {
				m_pPacket = nullptr;
			}
		}
	}

	if (m_pPacket != nullptr) {
		delete m_pPacket, m_pPacket = nullptr;
	}
}

} /* namespace syscil */
