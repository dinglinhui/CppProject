/*
 * OSDispatcher.cpp
 *
 *  Created on: Feb 4, 2015
 *      Author: dinglinhui
 */

#include "OSDispatcher.h"

namespace osext {

OSDispatcher::OSDispatcher() :
		OSThread() {
}

OSDispatcher::~OSDispatcher() {
}

int OSDispatcher::Create(int nTskOpt) {
	//
	OSInitHook();
	//
	SetOption(nTskOpt);
	return 0;
}

OSRet OSDispatcher::Start() {
	OSRet ret = OSThread::Start();
	if(ret != OSRet::OK)
		return ret;
	//
//	OSStart();
	return OSRet::OK;
}

OSRet OSDispatcher::Stop() {
	return OSRet::OK;
}

OSRet OSDispatcher::Pause() {
	return OSRet::OK;
}

OSRet OSDispatcher::Continue() {
	return OSRet::OK;
}

OSThread* OSDispatcher::GetThread(int nPrio) {
	return nullptr;
}

} /* namespace osext */
