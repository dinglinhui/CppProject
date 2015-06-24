#include <iostream>
#include "KDApp.h"

int main() {

	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(OS_THREAD_PAUSE));
	}
	return 0;
}
