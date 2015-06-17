/*
 * OSThreadEx.cpp
 *
 *  Created on: Feb 4, 2015
 *      Author: dinglinhui
 */

#include "OSThreadEx.h"

namespace OSExt {

OSThreadEx::OSThreadEx() :
		OSThread() {
}

OSThreadEx::~OSThreadEx() {
}

int OSThreadEx::Run() {
	std::cout << "OSThreadEx::Run" << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(OS_THREAD_PAUSE));
	return 0;
}

} /* namespace OSExt */
