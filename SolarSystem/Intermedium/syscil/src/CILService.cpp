/*
 * CILService.cpp
 *
 *  Created on: Jun 13, 2015
 *      Author: dinglinhui
 */

#include "../../syscil/inc/CILService.h"

namespace syscil {
CILService::CILService() :
		m_plstDevice(nullptr), m_pEntry(nullptr), m_pShedPool(nullptr) {
}

CILService::~CILService() {
}

void CILService::Install(CILDevice* pDevice) {
	m_plstDevice = pDevice;
}

void CILService::UnInstall(CILDevice* pDevice) {
}

void CILService::SetSchedPool(OSThreadPool* pPool) {
	m_pShedPool = pPool;
}

void CILService::SetPackEntry(OSMessageBase* ptr) {
	m_pEntry = ptr;
}

CILDevice* CILService::FindDevice(int nIdent) {
	return nullptr;
}

OSThreadPool* CILService::GetSchedPool(void) const {
	return m_pShedPool;
}

OSMessageBase* CILService::GetPackEntry(void) const {
	return m_pEntry;
}

int CILService::OSInitHook(void) {
	assert(m_pShedPool != NULL);
	return OSThreadEx::OSInitHook();
}

int CILService::ReceiveMessage(Message* msg) {
	return 0;
}

int CILService::OnHandleMessage(Message* msg) {
	return 0;
}

int CILService::Run() {
	while (true) {
		std::cout << "service" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(OS_THREAD_PAUSE));
	}
	return 0;
}

} /* namespace syscil */
