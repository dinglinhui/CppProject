/*
 * OSDispatcherEx.cpp
 *
 *  Created on: Jun 13, 2015
 *      Author: dinglinhui
 */

#include "OSDispatcherEx.h"

namespace osext {
OSDispatcherEx* OSDispatcherEx::m_pDispatcher = nullptr;

OSDispatcherEx::OSDispatcherEx() {
}

OSDispatcherEx::~OSDispatcherEx() {
}

OSRet OSDispatcherEx::Start(void) {
	return OSRet::OK;
}

OSRet OSDispatcherEx::Stop(void) {
	return OSRet::OK;
}

int OSDispatcherEx::Register(OSThreadEx* pService) {
	return 0;
}

int OSDispatcherEx::UnRegister(OSThreadEx* pService) {
	return 0;
}

void OSDispatcherEx::SendMessageToDescendants(Message* Message) {
}

void OSDispatcherEx::SendMessageToDescendants(DWORD nCmd, DWORD wParam, DWORD lParam) {
}

void OSDispatcherEx::PostMessageToDescendants(Message* Message) {
}

void OSDispatcherEx::PostMessageToDescendants(DWORD nCmd, DWORD wParam, DWORD lParam) {
}

OSThreadEx* OSDispatcherEx::Find(int nID) const {
	return nullptr;
}

void* OSDispatcherEx::GetMessagePtr(void) {
	return nullptr;
}

void OSDispatcherEx::PutMessagePtr(void* p) {
}

int OSDispatcherEx::Run(void) {
	return 0;
}

int OSDispatcherEx::OSInitHook(void) {
	return 0;
}

int OSDispatcherEx::OnHandleMessage(Message* Message) {
	return 0;
}

int OSDispatcherEx::ReceiveMessage(Message* Message) {
	return 0;
}

}
