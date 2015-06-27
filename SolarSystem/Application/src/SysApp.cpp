/*
 * SysApp.cpp
 *
 *  Created on: Jun 13, 2015
 *      Author: dinglinhui
 */

#include <SysApp.h>

namespace kding {

SysApp *SysApp::theApp = new SysApp();

SysApp *SysApp::GetAppInstance() {
	return SysApp::theApp;
}

SysApp::SysApp() :
		OSDispatcherEx(2048, 50, 500), m_objCIL(PRIO_CILSERVICE, 2048, 32), m_objSch(
				PRIO_SCHEDULE, 2048, 32), m_objDsp(PRIO_CILDSP, 5120, 32) {
	Initialize();
	this->Create(0);
	this->Start();
}

SysApp::~SysApp() {
}

OSRet SysApp::OSInitHook(void) {
	return OSDispatcherEx::OSInitHook();
}

int SysApp::OnHandleMessage(OSMessage* msg) {
	return 0;
}

int SysApp::Initialize(int* ret) {
	m_objDsp.SetDispatcher(this);
	m_objCIL.SetPackEntry(&m_objDsp);
	m_objCIL.SetDispatcher(this);
	m_objSch.SetDispatcher(this);
	return 0;
}

} /* namespace kding */
