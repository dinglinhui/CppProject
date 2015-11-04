/*
 * OSThreadPool.cpp
 *
 *  Created on: Feb 4, 2015
 *      Author: dinglinhui
 */

#include "OSThreadPool.h"

namespace osext {
OSThreadPool::OSThreadPool(int num) :
		m_num { num },
		m_maxQueueSize(0),
		m_running { false } {
}

OSThreadPool::~OSThreadPool() {
	if (m_running) {
		stop();
	}
}

void OSThreadPool::start() {
	assert(m_threads.empty());

	m_running = true;
	m_threads.reserve(m_num);
	for (int i = 0; i < m_num; i++) {
		m_threads.push_back(std::thread(std::bind(&OSThreadPool::threadFunc, this)));
	}
}

void OSThreadPool::stop() {
	std::unique_lock < std::mutex > ul { m_mutex };
	m_running = false;
	m_notEmpty.notify_all();

	for (auto &iter : m_threads) {
		iter.join();
	}
}

void OSThreadPool::run(const Task &t) {
	if (m_threads.empty()) {
		t();
	} else {
		std::unique_lock < std::mutex > ul { m_mutex };
		while (isFull()) {
			m_notFull.wait(ul);
		}
		assert(!isFull());
		m_taskQueue.push_back(t);
		m_notEmpty.notify_one();
	}
}

bool OSThreadPool::isFull() const {
	return m_maxQueueSize > 0 && m_taskQueue.size() >= m_maxQueueSize;
}

void OSThreadPool::threadFunc() {
	while (m_running) {
		Task task(take());
		if (task) {
			task();
		}
	}
}

Task OSThreadPool::take() {
	std::unique_lock < std::mutex > ul { m_mutex };
	while (m_taskQueue.empty() && m_running) {
		m_notEmpty.wait(ul);
	}

	Task task { };
	if (!m_taskQueue.empty()) {
		task = m_taskQueue.front();
		m_taskQueue.pop_front();
		if (m_maxQueueSize > 0) {
			m_notFull.notify_one();
		}
	}
	return task;
}

} /* namespace osext */

namespace test {
using namespace osext;

void fun() {
	std::cout << "[id:" << std::this_thread::get_id() << "] hello, world!" << std::endl;
}

void testOSThreadPool() {
	OSThreadPool pool(3);
	pool.setMaxQueueSize(100);
	pool.start();

	for (int i = 0; i < 20; i++) {
		pool.run(fun);
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}
}
