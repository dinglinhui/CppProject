/*
 * MonitorThread.cpp
 *
 *  Created on: Jun 13, 2015
 *      Author: dinglinhui
 */

#include "MonitorThread.h"

namespace osext {
MonitorThread::MonitorThread() {
}

MonitorThread::~MonitorThread() {
}

int MonitorThread::Run() {
	std::cout << "MonitorThread::Run" << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(OS_THREAD_PAUSE));
	return 0;
}

} /* namespace osext */
