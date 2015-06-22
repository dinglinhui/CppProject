/*
 * CILSchedule.cpp
 *
 *  Created on: Jun 13, 2015
 *      Author: dinglinhui
 */

#include "CILSchedule.h"

namespace syscil {
CILSchedule::CILSchedule() {
}

CILSchedule::~CILSchedule() {
}

int CILSchedule::Run() {
	OSHeartbeat * heartbeat = this->GetHeartbeat();
	while (true) {
		(*heartbeat)++;

		std::this_thread::sleep_for(std::chrono::milliseconds(OS_THREAD_PAUSE));
	}
	return 0;
}

} /* namespace syscil */
