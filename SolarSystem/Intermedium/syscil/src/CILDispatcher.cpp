/*
 * CILDispatcher.cpp
 *
 *  Created on: Jun 13, 2015
 *      Author: dinglinhui
 */

#include "CILDispatcher.h"

namespace syscil {

CILDispatcher::CILDispatcher() {
}

CILDispatcher::~CILDispatcher() {
}

OSRet CILDispatcher::Run() {
	OSHeartbeat &heartbeat = this->GetHeartbeat();
	while (true) {
		heartbeat++;
		std::cout << "2" << std::endl;

		std::this_thread::sleep_for(std::chrono::milliseconds(OS_THREAD_PAUSE));
	}
	return OSRet::OK;
}

} /* namespace syscil */
