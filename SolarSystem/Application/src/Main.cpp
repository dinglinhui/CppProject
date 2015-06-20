#include <iostream>
#include <cmath>
#include "OSThreadPool.h"
#include "ThreadPool.hpp"
#include "Events.hpp"
#include "DynClass.hpp"
//#include "CILService.h"
//#include "CILDispatcher.h"
//#include "CILSchedule.h"

int main() {

	test::testEvents();
	test::testThreadPool();
//	DYN_CLASS::test::testDynclass();
	return 0;
}
