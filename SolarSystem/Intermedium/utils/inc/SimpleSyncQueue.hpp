#ifndef SIMPLESYNCQUEUE_HPP_
#define SIMPLESYNCQUEUE_HPP_

#include <thread>
#include <condition_variable>
#include <mutex>
#include <list>
#include <iostream>

namespace utils {

template<typename T>
class SimpleSyncQueue {
public:
	void Put(const T& x) {
		std::lock_guard<std::mutex> locker(m_mutex);
		m_queue.push_back(x);
		m_notEmpty.notify_one();
	}

	void Take(T& x) {
		std::unique_lock<std::mutex> locker(m_mutex);
		m_notEmpty.wait(locker, [this] {return !m_queue.empty();});
		x = m_queue.front();
		m_queue.pop_front();
	}

private:
	std::list<T> m_queue;
	std::mutex m_mutex;
	std::condition_variable_any m_notEmpty;
};

}
#endif /* SIMPLESYNCQUEUE_HPP_ */
