/*
 * KDApp.cpp
 *
 *  Created on: Jun 13, 2015
 *      Author: dinglinhui
 */

#include "KDApp.h"

namespace kding {

KDApp *KDApp::theApp = new KDApp();

KDApp *KDApp::GetAppInstance() {
	return theApp;
}

KDApp::KDApp() {
	Initialize();
	this->Create(0);
	this->Start();
}

KDApp::~KDApp() {
}

OSRet KDApp::OSInitHook(void) {
	return OSDispatcherEx::OSInitHook();
}

int KDApp::OnHandleMessage(OSMessage* msg) {
	return 0;
}

int KDApp::Initialize(int* ret) {
	m_objDsp.SetDispatcher(this);
	m_objCIL.SetPackEntry(&m_objDsp);
	m_objCIL.SetDispatcher(this);
	m_objSch.SetDispatcher(this);
	return 0;
}

} /* namespace kding */
