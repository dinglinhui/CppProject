/*
 * OSThread.cpp
 *
 *  Created on: Feb 4, 2015
 *      Author: dinglinhui
 */

#include "OSThread.h"
#include "OSDefs.h"

namespace osext {

OSThread::OSThread(int nPrio, int nStackSize, INT16U nTskOpt) :
		thread_(nullptr),
		name_(""),
		status_(TStat::NoStart),
		prio_(nPrio),
		option_(nTskOpt) {
}

OSThread::OSThread(std::string name, int prio) :
		thread_(nullptr),
		name_(name),
		status_(TStat::NoStart),
		prio_(prio),
		option_(0) {
}

OSThread::~OSThread() {
	stop();
}

OSRet OSThread::start() {
	thread_ = new std::thread { std::bind(&OSThread::staticThreadFunction, this) };
	status_ = TStat::Starting;
	return OSRet::OK;
}

OSRet OSThread::stop() {
	status_ = TStat::Stopping;
	if (thread_ != nullptr) {
		if (thread_->joinable())
			thread_->join();
		delete thread_, thread_ = nullptr;
	}
	status_ = TStat::Stopped;
	return OSRet::OK;
}

OSRet OSThread::suspend(void) {
	return OSRet::OK;
}

OSRet OSThread::resume(void) {
	return OSRet::OK;
}

OSRet OSThread::OSInitHook(void) {
	return OSRet::OK;
}

OSRet OSThread::staticThreadFunction(void *param) {
	OSThread *pThis = (OSThread *) param;
	if (pThis == nullptr)
		return OSRet::NULLPTR;

	pThis->status_ = TStat::Started;

	if (pThis->OSInitHook() == OSRet::OK)
		pThis->run();

	pThis->stop();

	return OSRet::OK;
}

} /* namespace osext */
