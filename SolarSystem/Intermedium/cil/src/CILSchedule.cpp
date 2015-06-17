/*
 * CILSchedule.cpp
 *
 *  Created on: Jun 13, 2015
 *      Author: dinglinhui
 */

#include "CILSchedule.h"

namespace CIL {
CILSchedule::CILSchedule() {
}

CILSchedule::~CILSchedule() {
}

int CILSchedule::Run() {
	while (true) {
		std::cout << "schedule" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(OSExt::OS_THREAD_PAUSE));
	}
	return 0;
}

} /* namespace OSExt */
