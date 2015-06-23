/*
 * OSThread.cpp
 *
 *  Created on: Feb 4, 2015
 *      Author: dinglinhui
 */

#include "OSThread.h"
#include "OSDefs.h"

namespace osext {

OSThread::OSThread() :
		m_pThread(nullptr), m_nPrio(0), m_nOption(0) {
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

OSRet OSThread::OSInitHook(void) {
	return OSRet::OK;
}

OSRet OSThread::ThreadFunction(void *param) {
	OSThread *pThis = (OSThread *) param;
	if (pThis == nullptr)
		return OSRet::NULLPTR;

	if (pThis->OSInitHook() == OSRet::OK) {
		pThis->Run();
	}

	pThis->Stop();

	return OSRet::OK;
}

} /* namespace osext */
