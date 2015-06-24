/*
 * OSThread.cpp
 *
 *  Created on: Feb 4, 2015
 *      Author: dinglinhui
 */

#include "OSThread.h"
#include "OSDefs.h"

namespace osext {

	OSThread::OSThread() : thread_(nullptr), prio_(0), option_(0) {
}

OSThread::~OSThread() {
	Stop();
}

OSRet OSThread::Start() {
	thread_ = new std::thread { std::bind(&OSThread::ThreadFunction, this) };
	return OSRet::OK;
}

OSRet OSThread::Stop() {
	if (thread_ != nullptr){
		if (thread_->joinable())
			thread_->join();
		delete thread_, thread_ = nullptr;
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

	if (pThis->OSInitHook() == OSRet::OK) 
		pThis->Run();

	pThis->Stop();

	return OSRet::OK;
}

} /* namespace osext */
