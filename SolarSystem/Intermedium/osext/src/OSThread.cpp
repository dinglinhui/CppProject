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
		thread_(nullptr), name_(""), status_(TStat::NoStart), prio_(0), option_(0) {
}

OSThread::OSThread(std::string name, int prio) :
		thread_(nullptr), name_(name), status_(TStat::NoStart), prio_(prio), option_(0) {
}

OSThread::~OSThread() {
	Stop();
}

OSRet OSThread::Start() {
	thread_ = new std::thread { std::bind(&OSThread::ThreadFunction, this) };
	status_ = TStat::Starting;
	return OSRet::OK;
}

OSRet OSThread::Stop() {
	status_ = TStat::Stopping;
	if (thread_ != nullptr) {
		if (thread_->joinable())
			thread_->join();
		delete thread_, thread_ = nullptr;
	}
	status_ = TStat::Stopped;
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

	pThis->status_ = TStat::Started;

	if (pThis->OSInitHook() == OSRet::OK)
		pThis->Run();

	pThis->Stop();

	return OSRet::OK;
}

} /* namespace osext */
