/*
 * OSMessageBase.cpp
 *
 *  Created on: Jun 13, 2015
 *      Author: dinglinhui
 */

#include "OSMessageBase.h"

namespace osext {

extern void *OSGetMessagePtr(void);
extern void OSPutMessagePtr(void *p);

OSMessage::OSMessage(void) :
		m_pSource(nullptr), m_nCmd(MSGType::MSG_NULL), m_wParam(0), m_lParam(0), m_bAsyn(false), m_pACT(nullptr) {
}

void* OSMessage::operator new(size_t nSize) {
	if (nSize != sizeof(OSMessage)) {
		throw std::logic_error("OSMessage size error");
	}

	return OSGetMessagePtr();
}

void OSMessage::operator delete(void *p, size_t nSize) {
	if (nSize != sizeof(OSMessage)) {
		return;
	}

	OSPutMessagePtr(p);
}

OSMessageBase::OSMessageBase(int nQueueSize) :
		m_bOk(false), m_Queue(nQueueSize, sizeof(OSMessage)) {
}

OSMessageBase::~OSMessageBase(void) {
}

int OSMessageBase::PostMessage(OSMessageBase *pTarget, OSMessage *msg) {
	if (pTarget != nullptr && msg != nullptr) {
		msg->m_bAsyn = true;
		return pTarget->ReceiveMessage(msg);
	}
	return -1;
}

int OSMessageBase::PostMessage(OSMessageBase *pTarget, MSGType nCmd, DWORD wParam, DWORD lParam, void *act) {
	OSMessage msg;
	msg.m_pSource = this;
	msg.m_nCmd = nCmd;
	msg.m_wParam = wParam;
	msg.m_lParam = lParam;
	msg.m_pACT = act;
	return PostMessage(pTarget, &msg);
}

int OSMessageBase::SendMessage(OSMessageBase *pTarget, OSMessage *msg) {
	if (pTarget != nullptr && msg != nullptr) {
		msg->m_bAsyn = false;
		return pTarget->OnHandleMessage(msg);
	}
	return -1;
}

int OSMessageBase::SendMessage(OSMessageBase *pTarget, MSGType nCmd, DWORD wParam, DWORD lParam) {
	OSMessage msg;
	msg.m_pSource = this;
	msg.m_nCmd = nCmd;
	msg.m_wParam = wParam;
	msg.m_lParam = lParam;
	return SendMessage(pTarget, &msg);
}

int OSMessageBase::Write(OSMessage *msg) {
	int ret = 0;
	std::unique_lock<std::mutex> ul { m_mutex };
	ret = m_Queue.Write((void*) msg, 1);

	return ret;
}

int OSMessageBase::Read(OSMessage *msg) {
	int ret = 0;
	ret = m_Queue.Read((void*&) msg, 1);

	return ret;
}

bool OSMessageBase::QueueInitialize(void) {
	m_bOk = true;
	return true;
}

int OSMessageBase::Post(OSMessage *msg) {
//	return m_Queue.Post((void*) msg);
	return 0;
}
int OSMessageBase::Pend(OSMessage *&msg, DWORD dwWaitTime) {
//	return m_Queue.Pend((void*&) msg, dwWaitTime);
	return 0;
}

int OSMessageBase::ReceiveMessage(OSMessage *msg) {
	return Write(msg);
}

int OSMessageBase::OnHandleMessage(OSMessage *msg) {
	return -1;
}

} /* namespace osext */
