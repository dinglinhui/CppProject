/*
 * OSDispatcher.cpp
 *
 *  Created on: Feb 4, 2015
 *      Author: dinglinhui
 */

#include "OSDispatcher.h"

namespace OSExt {

OSDispatcher::OSDispatcher() :
		OSThread() {
}

OSDispatcher::~OSDispatcher() {
}

int OSDispatcher::Create(int nTskOpt) {
	return 0;
}

OSRet OSDispatcher::Start() {
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

} /* namespace OSExt */