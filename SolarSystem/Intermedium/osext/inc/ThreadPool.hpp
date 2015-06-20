/*
 * ThreadPool.hpp
 *
 *  Created on: Apr 8, 2015
 *      Author: dinglinhui
 */

#ifndef THREADPOOL_HPP_
#define THREADPOOL_HPP_

#include <vector>
#include <thread>
#include <functional>
#include <memory>
#include <atomic>
#include "OSVaries.hpp"
#include "SyncQueue.hpp"

namespace OSExt {
class ThreadPool {
public:
	using Task = std::function<void()>;
	ThreadPool(int numThreads = std::thread::hardware_concurrency()) :
			m_queue(OS_MAX_TASK_COUNT) {
		Start(numThreads);
	}

	~ThreadPool(void) {
		//如果没有停止时则主动停止线程池
		Stop();
	}

	void Stop() {
		//保证多线程情况下只调用一次StopThreadGroup
		std::call_once(m_flag, [this] {StopThreadGroup();});
	}

	void AddTask(Task&&task) {
		m_queue.Put(std::forward<Task>(task));
	}

	void AddTask(const Task& task) {
		m_queue.Put(task);
	}

private:
	void Start(int numThreads) {
		m_running = true;
		//创建线程组
		for (int i = 0; i < numThreads; ++i) {
			m_threadgroup.push_back(std::make_shared<std::thread>(&ThreadPool::RunInThread, this));
		}
	}

	void RunInThread() {
		while (m_running) {
			//取任务分别执行
			std::list<Task> list;
			m_queue.Take(list);

			for (auto& task : list) {
				if (!m_running)
					return;

				task();
			}
		}
	}

	void StopThreadGroup() {
		//让同步队列中的线程停止
		m_queue.Stop();
		//置为false，让内部线程跳出循环并退出
		m_running = false;
		//等待线程结束
		for (auto thread : m_threadgroup) {
			if (thread)
				thread->join();
		}
		m_threadgroup.clear();
	}

	//处理任务的线程组
	std::list<std::shared_ptr<std::thread>> m_threadgroup;
	//同步队列
	OSUtils::SyncQueue<Task> m_queue;
	//是否停止的标志
	std::atomic_bool m_running;
	std::once_flag m_flag;
};
} //namespace OSExt

namespace test {
void testThreadPool() {
	OSExt::ThreadPool pool;
	bool runing = true;
	std::thread thd1([&pool, &runing] {
		while(runing) {
			std::cout<<"produce "<<std::this_thread::get_id()<< std::endl;
			pool.AddTask([] {
						std::cout << "consume " << std::this_thread::get_id() << std::endl;
					});
		}
	});

	std::this_thread::sleep_for(std::chrono::seconds(10));
	runing = false;
	pool.Stop();

	thd1.join();
	getchar();
}
} //namespace test

#endif /* THREADPOOL_HPP_ */
