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

namespace multithreading {
class ThreadPool {
public:
    using Task = std::function<void()>;
    ThreadPool(unsigned int n = std::thread::hardware_concurrency()) {
        for (unsigned int i = 0; i < n; ++i)
            workers.emplace_back(std::bind(&ThreadPool::thread_proc, this));
    }

    ~ThreadPool() {
        std::call_once(onceflag, [this]
        {   stopThreadPool();});
    }

    template<typename F> void addTask(F&& f) {
        std::unique_lock<std::mutex> lock(queue_mutex);
        tasks.emplace_back(std::forward<F>(f));
        cv_task.notify_one();
    }

    void waitFinished() {
        std::unique_lock<std::mutex> lock(queue_mutex);
        cv_finished.wait(lock, [this]() {return tasks.empty() && (busy == 0);});
    }

    unsigned int getProcessed() const {
        return processed;
    }

private:
    void stopThreadPool() {
        // set stop-condition
        std::unique_lock<std::mutex> latch(queue_mutex);
        stop = true;
        cv_task.notify_all();
        latch.unlock();

        // all threads terminate, then we're done.
        for (auto& t : workers)
            t.join();
    }

    void thread_proc() {
        while (true) {
            std::unique_lock<std::mutex> latch(queue_mutex);
            cv_task.wait(latch, [this]() {return stop || !tasks.empty();});

            if (!tasks.empty()) {
                // got work. set busy.
                ++busy;

                // pull from queue
                auto task = tasks.front();
                tasks.pop_front();

                // release lock. run async
                latch.unlock();

                // run function outside context
                task();
                ++processed;

                latch.lock();
                --busy;
                cv_finished.notify_one();
            } else if (stop) {
                break;
            }
        }
    }

    std::vector<std::thread> workers;
    std::deque<Task> tasks;
    std::mutex queue_mutex;
    std::condition_variable cv_task;
    std::condition_variable cv_finished;
    std::atomic_uint processed = 0;
    std::once_flag onceflag;
    unsigned int busy = 0;
    bool stop = false;
};

void test_threadpool() {
    ThreadPool pool;
    unsigned int num = 0;

    // a cpu-busy task.
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
        // queue 100 work tasks
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
