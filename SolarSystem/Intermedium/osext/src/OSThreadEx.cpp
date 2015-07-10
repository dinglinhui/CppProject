/*
 * OSThreadEx.cpp
 *
 *  Created on: Feb 4, 2015
 *      Author: dinglinhui
 */

#include "OSThreadEx.h"

namespace osext {

OSThreadEx::OSThreadEx(int nPrio, int nStackSize, int nQueueBuffSize, INT16U nTskOpt) :
		OSThread(nPrio, nStackSize, nTskOpt),
		OSMessageBase(nQueueBuffSize),
		m_pParent(nullptr) {
}

OSThreadEx::~OSThreadEx() {
}

void OSThreadEx::SetDispatcher(OSDispatcherEx* pParent) {
	m_pParent = pParent;
	m_pParent->Register(this);
}

OSRet OSThreadEx::Start() {
	if (!QueueInitialize())
		return OSRet::ERROR;

	return OSThread::Start();
}

OSRet OSThreadEx::Run() {
	if (OSMessageBase::isOK()) {
		//Notify all thread FM_CREATE message
		PostMessage((OSMessageBase *) this, MSGType::MSG_CREATE, 0, 0);
		for (;;) {
			OSMessage *msg = nullptr;
			if (Pend(msg) == 0) {
				if (msg != nullptr) {
					int nRet = OnHandleMessage(msg);
					if (msg->m_pACT != nullptr) {
						PostMessage(msg->m_pSource, MSGType::MSG_ACT, nRet, (DWORD) msg->m_pACT, nullptr);
					}
					delete msg;
				}
			}
		}
	}
	return OSRet::OK;
}

} /* namespace osext */
