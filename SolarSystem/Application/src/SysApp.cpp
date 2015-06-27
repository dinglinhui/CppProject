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

SysApp::SysApp() {
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
