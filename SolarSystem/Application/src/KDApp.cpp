/*
 * KDApp.cpp
 *
 *  Created on: Jun 13, 2015
 *      Author: dinglinhui
 */

#include "KDApp.h"

namespace kding {
KDApp::KDApp() {
	Initialize();
}

KDApp::~KDApp() {
}

KDApp * KDApp::GetAppInstance() {
	return nullptr;
}

int KDApp::OSInitHook(void) {
	return 0;
}

int KDApp::OnHandleMessage(Message* msg) {
	return 0;
}

int KDApp::Initialize(int* ret) {
	return 0;
}

} /* namespace kding */
