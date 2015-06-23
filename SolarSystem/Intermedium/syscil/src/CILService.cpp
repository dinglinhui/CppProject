/*
 * CILService.cpp
 *
 *  Created on: Jun 13, 2015
 *      Author: dinglinhui
 */

#include <cassert>
#include "CILService.h"

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

OSRet CILService::OSInitHook(void) {
//	assert(m_pShedPool != nullptr);
	return OSThreadEx::OSInitHook();
}

int CILService::ReceiveMessage(OSMessage* msg) {
	return 0;
}

int CILService::OnHandleMessage(OSMessage* msg) {
//	assert(msg != nullptr);
	if( msg->m_nCmd == MSGType::CIL_SENDPACKET )
	{
		CILDevice *pVxd = FindDevice(msg->m_wParam);
		if( pVxd != nullptr )
		{
			if( pVxd->Usable() )
			{
				return pVxd->Send((CILPacket*)msg->m_lParam, msg->m_pSource);
			}
		}
		assert(false );
	}
	return -1;
}

OSRet CILService::Run() {
//	OSHeartbeat &heartbeat = this->GetHeartbeat();
	while (true) {
//		heartbeat++;
		std::cout << "1" << std::endl;

		std::this_thread::sleep_for(std::chrono::milliseconds(OS_THREAD_PAUSE));
	}
	return OSRet::OK;
}

} /* namespace syscil */
