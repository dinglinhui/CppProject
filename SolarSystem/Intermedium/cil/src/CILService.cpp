/*
 * CILService.cpp
 *
 *  Created on: Jun 13, 2015
 *      Author: dinglinhui
 */

#include "CILService.h"

namespace CIL {
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

void CILService::SetSchedPool(OSExt::OSThreadPool* pPool) {
	m_pShedPool = pPool;
}

void CILService::SetPackEntry(OSExt::OSMessageBase* ptr) {
	m_pEntry = ptr;
}

CILDevice* CILService::FindDevice(int nIdent) {
	return nullptr;
}

OSExt::OSThreadPool* CILService::GetSchedPool(void) const {
	return m_pShedPool;
}

OSExt::OSMessageBase* CILService::GetPackEntry(void) const {
	return m_pEntry;
}

int CILService::OSInitHook(void) {
	assert(m_pShedPool != NULL);
	return OSExt::OSThreadEx::OSInitHook();
}

int CILService::ReceiveMessage(OSExt::Message* msg) {
	return 0;
}

int CILService::OnHandleMessage(OSExt::Message* msg) {
	return 0;
}

int CILService::Run() {
	while (true) {
		std::cout << "service" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(OS_THREAD_PAUSE));
	}
	return 0;
}

} /* namespace OSExt */
