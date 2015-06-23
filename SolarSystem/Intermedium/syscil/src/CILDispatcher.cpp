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

int CILDispatcher::Run() {
	OSHeartbeat &heartbeat = this->GetHeartbeat();
	while (true) {
		heartbeat++;
		std::cout << "CILDispatcher" << std::endl;

		std::this_thread::sleep_for(std::chrono::milliseconds(OS_THREAD_PAUSE));
	}
	return 0;
}

} /* namespace syscil */
