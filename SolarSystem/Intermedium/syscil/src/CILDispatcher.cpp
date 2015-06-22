/*
 * CILDispatcher.cpp
 *
 *  Created on: Jun 13, 2015
 *      Author: dinglinhui
 */

#include "../../syscil/inc/CILDispatcher.h"

namespace syscil {

CILDispatcher::CILDispatcher() {
}

CILDispatcher::~CILDispatcher() {
}

int CILDispatcher::Run() {
	while (true) {
		std::cout << "dispatcher" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(OS_THREAD_PAUSE));
	}
	return 0;
}

} /* namespace syscil */