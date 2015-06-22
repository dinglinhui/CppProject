/*
 * Timer.hpp
 *
 *  Created on: Apr 8, 2015
 *      Author: dinglinhui
 */

#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <string>

namespace utils {

using namespace std;
using namespace std::chrono;

class Timer {
public:
	Timer() :
			m_begin(high_resolution_clock::now()) {
	}
	void reset() {
		m_begin = high_resolution_clock::now();
	}

	//默认输出秒
	double elapsed() const {
		return duration_cast<duration<double>>(high_resolution_clock::now() - m_begin).count();
	}
	//默认输出毫秒
	int64_t elapsed_milli() const {
		return duration_cast<chrono::milliseconds>(high_resolution_clock::now() - m_begin).count();
	}

	//微秒
	int64_t elapsed_micro() const {
		return duration_cast<chrono::microseconds>(high_resolution_clock::now() - m_begin).count();
	}

	//纳秒
	int64_t elapsed_nano() const {
		return duration_cast<chrono::nanoseconds>(high_resolution_clock::now() - m_begin).count();
	}

	//秒
	int64_t elapsed_seconds() const {
		return duration_cast<chrono::seconds>(high_resolution_clock::now() - m_begin).count();
	}

	//分
	int64_t elapsed_minutes() const {
		return duration_cast<chrono::minutes>(high_resolution_clock::now() - m_begin).count();
	}

	//时
	int64_t elapsed_hours() const {
		return duration_cast<chrono::hours>(high_resolution_clock::now() - m_begin).count();
	}

private:
	time_point<high_resolution_clock> m_begin;
};
} //namespace utils

namespace test {

using namespace std;
using namespace std::chrono;
using namespace utils;

void fun() {
	std::cout << "hello word" << std::endl;
}

void testTimer() {
	Timer timer; //开始计时
	fun();
	std::cout << timer.elapsed() << std::endl; //打印fun函数耗时多少毫秒
	std::cout << timer.elapsed_milli() << std::endl; //打印毫秒
	std::cout << timer.elapsed_micro() << std::endl; //打印微秒
	std::cout << timer.elapsed_nano() << std::endl; //打印纳秒
	std::cout << timer.elapsed_seconds() << std::endl; //打印秒
	std::cout << timer.elapsed_minutes() << std::endl; //打印分钟
	std::cout << timer.elapsed_hours() << std::endl; //打印小时

	auto t = chrono::system_clock::to_time_t(chrono::system_clock::now());
	std::cout << std::put_time(std::localtime(&t), "%Y-%m-%d %X") << std::endl;
	std::cout << std::put_time(std::localtime(&t), "%Y-%m-%d %H.%M.%S") << std::endl;

	system_clock::time_point now = system_clock::now();
	std::time_t last = system_clock::to_time_t(now - chrono::hours(24));
	std::time_t next = system_clock::to_time_t(now + chrono::hours(24));
	std::cout << "One day ago, the time was " << std::put_time(std::localtime(&last), "%F %T") << '\n';
	std::cout << "Next day, the time was " << std::put_time(std::localtime(&next), "%F %T") << '\n';

	system_clock::time_point now1 = system_clock::now();
	std::cout << "Hello World\n";
	system_clock::time_point now2 = system_clock::now();
	std::cout << (now2 - now1).count() << " tick count" << std::endl;
	std::cout << duration_cast<microseconds>(now2 - now1).count() << " microseconds " << std::endl;

	typedef duration<int, std::ratio<60 * 60 * 24>> days_type;
	time_point<system_clock, days_type> today = time_point_cast<days_type>(system_clock::now());
	std::cout << today.time_since_epoch().count() << " days since epoch" << std::endl;

	chrono::milliseconds ms { 3 }; // 3 毫秒
	// 6000 microseconds constructed from 3 milliseconds
	chrono::microseconds us = 2 * ms; //6000微秒
	// 30Hz clock using fractional ticks
	chrono::duration<double, std::ratio<1, 30>> hz30(3.5);
	std::cout << "3 ms duration has " << ms.count() << " ticks\n" << "6000 us duration has " << us.count()
			<< " ticks\n";

	chrono::minutes t1(10);
	chrono::seconds t2(60);
	chrono::seconds t3 = t1 - t2;
	std::cout << t3.count() << " second" << std::endl;
	std::cout << chrono::duration_cast<chrono::minutes>(t3).count() << " minutes" << std::endl;
	std::cout << std::endl;
}

} //namespace test
#endif /* TIMER_HPP_ */
