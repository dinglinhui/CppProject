/*
 * OSThreadPool.h
 *
 *  Created on: Feb 4, 2015
 *      Author: dinglinhui
 */

#ifndef OSTHREADPOOL_H_
#define OSTHREADPOOL_H_
#include <iostream>
#include <cstdlib>
#include <functional>
#include <thread>
#include <string>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <memory>
#include <cassert>
#include <algorithm>
#include <queue>

namespace OSExt {
typedef std::function<void()> Task;

class OSThreadPool {
public:
	OSThreadPool(int num);
	~OSThreadPool();

	OSThreadPool(OSThreadPool &&) = delete;
	OSThreadPool(const OSThreadPool &) = delete;
	void operator=(OSThreadPool &&) = delete;
	void operator=(const OSThreadPool &) = delete;

	void setMaxQueueSize(int maxSize) {
		m_maxQueueSize = maxSize;
	}

	void start();
	void stop();
	void run(const Task &t);

private:
	bool isFull() const;
	void threadFunc();
	Task take();

private:
	int m_num;
	std::mutex m_mutex;
	std::condition_variable m_notEmpty;
	std::condition_variable m_notFull;
	std::vector<std::thread> m_threads;
	std::deque<Task> m_taskQueue;
	size_t m_maxQueueSize;
	bool m_running;
};

} /* namespace OSExt */

#endif /* OSTHREADPOOL_H_ */
