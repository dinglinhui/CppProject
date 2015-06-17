/*
 * OSThread.cpp
 *
 *  Created on: Feb 4, 2015
 *      Author: dinglinhui
 */

#include "OSThread.h"
#include "OSDefs.h"

namespace OSExt {

OSThread::OSThread() {
}

OSThread::~OSThread() {
	Stop();
}

OSRet OSThread::Start() {
	m_pThread = new std::thread { std::bind(&OSThread::ThreadFunction, this) };
	return OSRet::OK;
}

OSRet OSThread::Stop() {
	if (m_pThread != nullptr) {
		if (m_pThread->joinable())
			m_pThread->join();

		delete m_pThread, m_pThread = nullptr;
	}
	return OSRet::OK;
}

OSRet OSThread::Pause(void) {
	return OSRet::OK;
}

OSRet OSThread::Continue(void) {
	return OSRet::OK;
}

int OSThread::OSInitHook(void) {
	return OS_NO_ERR;
}

int OSThread::ThreadFunction(void *param) {
	OSThread *pThis = (OSThread *) param;
	if (pThis == nullptr)
		return 1;

	pThis->Run();

	return 0;
}

} /* namespace OSExt */
