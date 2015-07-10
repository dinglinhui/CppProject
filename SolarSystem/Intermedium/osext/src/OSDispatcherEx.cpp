/*
 * OSDispatcherEx.cpp
 *
 *  Created on: Jun 13, 2015
 *      Author: dinglinhui
 */

#include "OSDispatcherEx.h"
#include <cassert>
#include <cstdlib>
#include <stdexcept>

namespace osext {
utils::MemPool<OSMessage> OSDispatcherEx::mempool_(OS_MAX_MESSAGE_NUM); //预先分配10个OSMessage的内存空间

OSDispatcherEx* OSDispatcherEx::m_pDispatcher = nullptr;

////////////////////////////////////////////////////////////////////////////////
//
// global function
//
////////////////////////////////////////////////////////////////////////////////
OSDispatcherEx* GetOSDispatcher(void) {
	return OSDispatcherEx::m_pDispatcher;
}

int OSSendMessage(int nPrio, MSGType nCmd, DWORD wParam, DWORD lParam) {
	OSDispatcherEx *pDispatcher = GetOSDispatcher();
	if (pDispatcher == nullptr)
		return 1;

	if (nCmd == MSGType::MSG_NULL) {
		return pDispatcher->SendMessage((OSMessageBase*) pDispatcher, nCmd, wParam, lParam);
	} else {
		OSThreadEx *pThread = pDispatcher->Find(std::this_thread::get_id());
		if (pThread != nullptr) {
			return pDispatcher->SendMessage((OSMessageBase*) pThread, nCmd, wParam, lParam);
		}
	}
	return 3;
}

int OSPostMessage(int nPrio, MSGType nCmd, DWORD wParam, DWORD lParam) {
	OSDispatcherEx *pDispatcher = GetOSDispatcher();
	if (pDispatcher != nullptr) {
		MSGType nNewCmd = MSGType::MSG_ACT;
		return pDispatcher->PostMessage(pDispatcher, nNewCmd, wParam, lParam);
	}
	return -1;
}

void *OSGetMessagePtr(void) {
	OSDispatcherEx *pDispatcher = GetOSDispatcher();
	if (pDispatcher != nullptr)
		return pDispatcher->GetMessagePtr();
	return nullptr;
}

void OSPutMessagePtr(void *p) {
	OSDispatcherEx *pDispatcher = GetOSDispatcher();
	if (pDispatcher != nullptr)
		pDispatcher->PutMessagePtr(p);
}

////////////////////////////////////////////////////////////////////////////////
//
// class OSDispatcherEx
//
////////////////////////////////////////////////////////////////////////////////
OSDispatcherEx::OSDispatcherEx(int nStackSize, int nQueueBuffSize, int nMaxMSGCount) :
		OSDispatcher(nStackSize),
		OSMessageBase(nQueueBuffSize) {
}

OSDispatcherEx::~OSDispatcherEx() {
}

OSRet OSDispatcherEx::Start(void) {
	if (!QueueInitialize())
		return OSRet::ERROR;

	OSDispatcherEx::m_pDispatcher = this;
	return OSDispatcher::Start();
}

OSRet OSDispatcherEx::Stop(void) {
	threads_.clear();
	OSDispatcherEx::m_pDispatcher = nullptr;
	return OSRet::OK;
}

int OSDispatcherEx::Register(OSThreadEx* pThreadEx) {
	threads_.push_back(pThreadEx);
	return 0;
}

int OSDispatcherEx::UnRegister(OSThreadEx* pThreadEx) {
	OSThreadEx * thread = Find(pThreadEx->GetThreadID());
	threads_.remove(thread);
	return 0;
}

OSThreadEx* OSDispatcherEx::Find(std::thread::id nID) const {
	OSThreadEx * thread = nullptr;
	std::for_each(std::begin(threads_), std::end(threads_), [nID, &thread](OSThreadEx *pThreadEx) {
		if (nID == pThreadEx->GetThreadID()) thread = pThreadEx;
	});

	return thread;
}

void OSDispatcherEx::SendMessageToDescendants(OSMessage* msg) {
	if (msg != nullptr) {
		SendMessageToDescendants(msg->m_nCmd, msg->m_wParam, msg->m_lParam);
		delete msg;
	}
}

void OSDispatcherEx::SendMessageToDescendants(MSGType nCmd, DWORD wParam, DWORD lParam) {
	std::for_each(std::begin(threads_), std::end(threads_), [this, nCmd, wParam, lParam](OSThreadEx *pThreadEx) {
		SendMessage(pThreadEx, nCmd, wParam, lParam);
	});
}

void OSDispatcherEx::PostMessageToDescendants(OSMessage* msg) {
	if (msg != nullptr) {
		PostMessageToDescendants(msg->m_nCmd, msg->m_wParam, msg->m_lParam);
		delete msg;
	}
}

void OSDispatcherEx::PostMessageToDescendants(MSGType nCmd, DWORD wParam, DWORD lParam) {
	std::for_each(std::begin(threads_), std::end(threads_), [this, nCmd, wParam, lParam](OSThreadEx *pThreadEx) {
		PostMessage(pThreadEx, nCmd, wParam, lParam);
	});
}

void* OSDispatcherEx::GetMessagePtr(void) {
	return mempool_.alloc();
}

void OSDispatcherEx::PutMessagePtr(void* p) {
	mempool_.dealloc((OSMessage*) p);
}

void OSDispatcherEx::ScanThreads(void) {
	std::for_each(std::begin(threads_), std::end(threads_), [](OSThreadEx *pThreadEx) {
		OSHeartbeat &heartbeat = pThreadEx->GetHeartbeat();
		if (heartbeat.isEqual()) throw std::logic_error("thread heartbeat error");
		else heartbeat--;
	});
}

OSRet OSDispatcherEx::Run(void) {
	//Notify all thread FM_CREATE message
	PostMessage((OSMessageBase *) this, MSGType::MSG_CREATE, 0, 0);
	//
	while (true) {
		try {
			std::this_thread::sleep_for(std::chrono::milliseconds(OS_MONITOR_THREAD_PAUSE));
			//
			OSMessage *msg = nullptr;
			if (Pend(msg) == 0) {
				if (msg != nullptr) {
					OnHandleMessage(msg);
					delete msg;
				}
			}
			//
			ScanThreads();
		} catch (std::exception const& ex) {
			std::cerr << "Exception: " << ex.what() << std::endl;
		}
	}
	return OSRet::OK;
}

OSRet OSDispatcherEx::OSInitHook(void) {
	std::for_each(std::begin(threads_), std::end(threads_), [this](OSThreadEx *pThreadEx) {
		if (pThreadEx->Start() != OSRet::OK) throw std::logic_error(pThreadEx->GetThreadName() + "start error");
	});
	return OSDispatcher::OSInitHook();
}

int OSDispatcherEx::OnHandleMessage(OSMessage* msg) {
	int nRet = -1;

	if (msg != nullptr) {
		switch (msg->m_nCmd) {
		case MSGType::MSG_GETSERVICE: {
			OSThreadEx* pService = Find(std::this_thread::get_id());
			if (msg->m_bAsyn) {
				PostMessage((OSMessageBase*) msg->m_pSource, msg->m_nCmd, msg->m_wParam, (DWORD) pService, nullptr);
			} else {
				msg->m_lParam = (DWORD) pService;
			}
		}
			break;

		default:
			nRet = OSMessageBase::OnHandleMessage(msg);
			break;
		}
		return nRet;
	}
	return -1;
}

int OSDispatcherEx::ReceiveMessage(OSMessage* msg) {
	if (msg == nullptr)
		return 1;

	BYTE nPrio = 0;
//	BYTE nPrio = (BYTE) ((msg->m_nCmd & 0xFF000000) >> 24);
//	msg->m_nCmd &= 0x00FFFFFF;

	int nRet = 2;
	if (nPrio == 0) {
		nRet = OSMessageBase::ReceiveMessage(msg);
	} else {
		if (nPrio == 0xFF) {
			PostMessageToDescendants(msg);
			nRet = 0;
		} else {
			OSThreadEx *pThread = Find(std::this_thread::get_id());
			if (pThread != nullptr) {
				nRet = PostMessage((OSMessageBase*) pThread, msg);
			}
		}
	}

	return nRet;
}
}
