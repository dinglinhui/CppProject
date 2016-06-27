#include <SysApp.h>
#include <iostream>

int main() {

	//handle all the thread and rollback resources
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(OS_THREAD_PAUSE));
	}
	return 0;
}
