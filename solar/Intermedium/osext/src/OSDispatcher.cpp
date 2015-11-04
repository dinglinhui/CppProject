/*
 * OSDispatcher.cpp
 *
 *  Created on: Feb 4, 2015
 *      Author: dinglinhui
 */

#include "OSDispatcher.h"

namespace osext {

OSDispatcher::OSDispatcher(int nStackSize) :
		OSThread(0, nStackSize, 0) {
}

OSDispatcher::~OSDispatcher() {
}

int OSDispatcher::Create(int nTskOpt) {
	//
	SetOption(nTskOpt);
	return 0;
}

//OSRet OSDispatcher::Start() {
//	return OSThread::Start();
//}
//
//OSRet OSDispatcher::Stop() {
//	return OSThread::Stop();
//}
//
//OSRet OSDispatcher::Pause() {
//	return OSThread::Pause();
//}
//
//OSRet OSDispatcher::Continue() {
//	return OSThread::Continue();
//}

OSThread* OSDispatcher::GetThread(int nPrio) {
	return nullptr;
}

} /* namespace osext */
