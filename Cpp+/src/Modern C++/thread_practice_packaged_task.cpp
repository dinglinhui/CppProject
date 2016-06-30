/*#include <iostream>
#include <future>
#include <thread>
#include <cstdint>
#include <stdexcept>
#include <limits>
#include <utility>

std::uint32_t add(std::uint32_t a, std::uint32_t b) {
	std::cout << "thread id = " << std::this_thread::get_id() << ", a = " << a
			<< ", b = " << b << std::endl;
	if (std::numeric_limits<std::uint32_t>::max() - a < b) {
		throw std::overflow_error("overflow_error");
	}
	return a + b;
}

int main() {
	// 使用std::packaged_task包装add函数
	std::packaged_task<std::uint32_t(std::uint32_t, std::uint32_t)> task1(add);
	// 取得std::future用于获取异步操作的结果
	std::future<std::uint32_t> f1 = task1.get_future();
	// 将task1对象作为线程的函数执行体
	std::thread(std::move(task1), 1ul, 2ul).detach();
	// 通过std::future对象获取异步操作的结果
	std::uint32_t sum1 = f1.get();
	std::cout << "thread id = " << std::this_thread::get_id() << ", sum1 = "
			<< sum1 << std::endl;
	std::packaged_task<std::uint32_t(std::uint32_t, std::uint32_t)> task2(add);
	std::future<std::uint32_t> f2 = task2.get_future();
	// 4000000000ul + 4000000000ul会抛出异常
	std::thread(std::move(task2), 4000000000ul, 4000000000ul).detach();
	try {
		std::uint32_t sum2 = f2.get();
		std::cout << "thread id = " << std::this_thread::get_id() << ", sum2 = "
				<< sum2 << std::endl;
	} catch (const std::overflow_error& e) {
		std::cout << "thread id = " << std::this_thread::get_id()
				<< ", e.what() = " << e.what() << std::endl;
	}
}*/
