/*
 * OSMessageBase.cpp
 *
 *  Created on: Jun 13, 2015
 *      Author: dinglinhui
 */

#include "OSMessageBase.h"

namespace osext {

Message::Message(void) :
		m_pSource(nullptr), m_nCmd(0), m_wParam(0), m_lParam(0), m_bAsyn(false), m_pACT(nullptr) {
}

OSMessageBase::OSMessageBase(int nQueueSize) :
		m_bOk(false), m_Queue(nQueueSize, sizeof(Message)) {
}

OSMessageBase::~OSMessageBase(void) {
}

int OSMessageBase::PostMessage(OSMessageBase *pTarget, Message *msg) {
	if (pTarget != nullptr && msg != nullptr) {
		msg->m_bAsyn = true;
		return pTarget->ReceiveMessage(msg);
	}
	return -1;
}

int OSMessageBase::PostMessage(OSMessageBase *pTarget, DWORD nCmd, DWORD wParam, DWORD lParam, void *act) {
	Message msg;
	msg.m_pSource = this;
	msg.m_nCmd = nCmd;
	msg.m_wParam = wParam;
	msg.m_lParam = lParam;
	msg.m_pACT = act;
	return PostMessage(pTarget, &msg);
}

int OSMessageBase::SendMessage(OSMessageBase *pTarget, Message *msg) {
	if (pTarget != nullptr && msg != nullptr) {
		msg->m_bAsyn = false;
		return pTarget->OnHandleMessage(msg);
	}
	return -1;
}

int OSMessageBase::SendMessage(OSMessageBase *pTarget, DWORD nCmd, DWORD wParam, DWORD lParam) {
	Message msg;
	msg.m_pSource = this;
	msg.m_nCmd = nCmd;
	msg.m_wParam = wParam;
	msg.m_lParam = lParam;
	return SendMessage(pTarget, &msg);
}

int OSMessageBase::Write(Message *msg) {
	int ret = 0;
	std::unique_lock<std::mutex> ul { m_mutex };
	ret = m_Queue.Write((void*) msg, 1);

	return ret;
}

int OSMessageBase::Read(Message *msg) {
	int ret = 0;
	ret = m_Queue.Read((void*&) msg, 1);

	return ret;
}

bool OSMessageBase::QueueInitialize(void) {
	m_bOk = true;
	return true;
}

int OSMessageBase::ReceiveMessage(Message *msg) {
	return Write(msg);
}

int OSMessageBase::OnHandleMessage(Message *msg) {
	return -1;
}

} /* namespace osext */
