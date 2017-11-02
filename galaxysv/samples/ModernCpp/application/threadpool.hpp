#if 1
#ifndef SRC_THREADPOOL_H_
#define SRC_THREADPOOL_H_

#include <iostream>
#include <deque>
#include <functional>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <random>
#include <algorithm>
#include <atomic>
#include <iterator>

namespace application {

class ThreadPool {
public:
    using Task = std::function<void()>;
    ThreadPool(unsigned int n = std::thread::hardware_concurrency()) {
        for (unsigned int i = 0; i < n; ++i)
            _workers.emplace_back(std::bind(&ThreadPool::thread_proc, this));
    }

    ~ThreadPool() {
        std::call_once(_once_flag, [this]
        {   stopThreadPool();});
    }

    template<typename F> void addTask(F&& f) {
        std::unique_lock<std::mutex> lock(_mutex);
        _tasks.emplace_back(std::forward<F>(f));
        _cv_task.notify_one();
    }

    void waitFinished() {
        std::unique_lock<std::mutex> lock(_mutex);
        _cv_finished.wait(lock, [this]() {return _tasks.empty() && (_busy == 0);});
    }

    unsigned int getProcessed() const {
        return _processed;
    }

private:
    void stopThreadPool() {
        // set _stop-condition
        std::unique_lock<std::mutex> latch(_mutex);
        _stop = true;
        _cv_task.notify_all();
        latch.unlock();

        // all threads terminate, then we're done.
        for (auto& t : _workers)
            t.join();
    }

    void thread_proc() {
        while (true) {
            std::unique_lock<std::mutex> latch(_mutex);
            _cv_task.wait(latch, [this]() {return _stop || !_tasks.empty();});

            if (!_tasks.empty()) {
                // got work. set _busy.
                _busy++;

                // pull from queue
                auto task = _tasks.front();
                _tasks.pop_front();

                // release lock. run async
                latch.unlock();

                // run function outside context
                task();
                _processed++;

                latch.lock();
                _busy--;
                _cv_finished.notify_one();
            } else if (_stop) {
                break;
            }
        }
    }

    std::vector<std::thread> _workers;
    std::deque<Task> _tasks;
    std::mutex _mutex;
    std::condition_variable _cv_task;
    std::condition_variable _cv_finished;
    std::atomic_uint _processed = 0;
    std::once_flag _once_flag;
    unsigned int _busy = 0;
    bool _stop = false;
};

void test_threadpool() {
    ThreadPool pool;
    unsigned int num = 0;

    // a cpu-_busy task.
    auto task = [&]() {
        std::random_device rd;
        std::mt19937 rng(rd());

        std::vector<int> data;
        data.reserve(100000);
        std::generate_n(std::back_inserter(data), data.capacity(), [&]() {return rng();});
        std::sort(data.begin(), data.end(), std::greater<int>());
        num++;
    };

    // run five batches of 100 items
    for (int x = 0; x < 5; ++x) {
        // queue 100 work _tasks
        for (int i = 0; i < 100; ++i)
            pool.addTask(task);

        pool.waitFinished();
        std::cout << pool.getProcessed() << '\n';
    }
    std::cout << num << '\n';
}

} /* namespace multithreading */

#endif /* SRC_THREADPOOL_H_ */
#endif
