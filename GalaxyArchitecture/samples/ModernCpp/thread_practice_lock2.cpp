#include <atomic>
#include <thread>
#include <mutex>
#include <iostream>

class spin_mutex {
	std::atomic_flag flag = ATOMIC_FLAG_INIT;
public:
	spin_mutex() = default;
	spin_mutex(const spin_mutex&) = delete;
	spin_mutex& operator=(const spin_mutex&) = delete;
	void lock() {
		while (flag.test_and_set(std::memory_order_acquire))
			;
	}
	void unlock() {
		flag.clear(std::memory_order_release);
	}
};

int num = 0;
spin_mutex sm;

void thread_proc() {

	for (int i = 0; i < 100000; ++i) {
		std::lock_guard<spin_mutex> lock(sm);
		++num;
	}
}

/*int main() {
	std::thread td1(thread_proc), td2(thread_proc);
	td1.join();
	td2.join();
	std::cout << num << std::endl;
	return 0;
}*/
