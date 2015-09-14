/*
 * main.cpp
 *
 *  Created on: Jun 4, 2015
 *      Author: dlh
 */

#include "Global.hpp"
#include "Point.hpp"
#include "Events.hpp"
#include "Timer.hpp"
#include "Range.hpp"
#include "Optional.hpp"
#include "CommCommand.hpp"
#include "ObjectPool.hpp"
#include "ThreadPool.hpp"

int main() {

	try {
//		float coords[cplusplus::global::three_dimension] = { 1.2, 3.4, 5.6 };
//		cplusplus::test::testPoint(coords);
//		cplusplus::test::testEvents();
//		cplusplus::test::testTimer();
//		cplusplus::test::testRange();
//		cplusplus::test::testOptional();
//		cplusplus::test::testWrap();
//		cplusplus::test::testObjectPool();
		cplusplus::test::testThreadPool();

	} catch (std::exception const& ex) {
		std::cerr << "Exception: " << ex.what() << std::endl;
		return -1;
	}

	return 0;
}
