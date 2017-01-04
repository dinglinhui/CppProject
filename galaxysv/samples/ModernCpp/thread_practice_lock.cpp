/*
#include <iostream>
#include <future>
#include <thread>
#include <cstdint>
#include <stdexcept>
#include <limits>
#include <utility>
#include <set>
#include <random>
#include <atomic>
#include <exception>

class spin_mutex {
	std::atomic<bool> flag = ATOMIC_VAR_INIT(false);
public:
	spin_mutex() = default;
	spin_mutex(const spin_mutex&) = delete;
	spin_mutex& operator=(const spin_mutex&) = delete;
	void lock() {
		bool expected = false;
		while (!flag.compare_exchange_strong(expected, true))
			expected = false;
	}
	void lock() {
	  bool expected = false;
	  while(!flag.compare_exchange_strong(expected, true, std::memory_order_acquire, std::memory_order_relaxed))
	    expected = false;
	}
	void unlock() {
		flag.store(false);
	}
};

int num = 0;
spin_mutex sm;

void thread_proc() {
	for (int i = 0; i < 100000; ++i) {
		sm.lock();
		++num;
		sm.unlock();
	}
}

int main() {
//	#1
#if 1
	std::set<int> int_set;
	std::mutex mt;
	auto f = [&int_set, &mt]() {
		try {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dis(1, 1000);
			for(std::size_t i = 0; i != 100000; ++i) {
				mt.lock();
				int_set.insert(dis(gen));
				mt.unlock();
			}
		} catch(...) {}
	};
	std::thread td1(f), td2(f);
	td1.join();
	td2.join();
#elif
//	#2
	std::set<int> int_set;
	std::mutex mt;
	auto f = [&int_set, &mt]() {
		try {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dis(1, 1000);
			for(std::size_t i = 0; i != 100000; ++i) {
				std::lock_guard<std::mutex> lck(mt);
				int_set.insert(dis(gen));
			}
		} catch(...) {}
	};
	std::thread td1(f), td2(f);
	td1.join();
	td2.join();
#endif

	std::thread td_1(thread_proc), td_2(thread_proc);
	td_1.join();
	td_2.join();
	std::cout << num << std::endl;
}
*/
