/*
#include <iostream>
#include <future>
#include <thread>
#include <cstdint>
#include <stdexcept>
#include <limits>
#include <utility>
#include <exception>

std::uint32_t add(std::uint32_t a, std::uint32_t b) {
	std::cout << "thread id = " << std::this_thread::get_id() << ", a = " << a
			<< ", b = " << b << std::endl;
	if (std::numeric_limits<std::uint32_t>::max() - a < b) {
		throw std::overflow_error("overflow_error");
	}
	return a + b;
}

// add函数的包装函数
void add_wrapper(std::promise<std::uint32_t> promise, std::uint32_t a,
		std::uint32_t b) {
	try {
		// 设置值为供std::future对象获取
		promise.set_value(add(a, b));
	} catch (...) {
		// 设置异常在std::future获取值时抛出
		promise.set_exception(std::current_exception());
	}
}

int main() {
	// 创建std::promise对象
	std::promise<std::uint32_t> promise1;
	// 获取关联的std::future对象
	std::future<std::uint32_t> f1 = promise1.get_future();
	// 启动线程执行add函数的包装函数
	std::thread(add_wrapper, std::move(promise1), 1ul, 2ul).detach();
	// 等待并获取异步操作的结果
	std::uint32_t sum1 = f1.get();
	std::cout << "thread id = " << std::this_thread::get_id() << ", sum1 = "
			<< sum1 << std::endl;
	std::promise<std::uint32_t> promise2;
	std::future<std::uint32_t> f2 = promise2.get_future();
	// 4000000000ul + 4000000000ul会抛出异常
	std::thread(add_wrapper, std::move(promise2), 4000000000ul, 4000000000ul).detach();
	try {
		std::uint32_t sum2 = f2.get();
		std::cout << "thread id = " << std::this_thread::get_id() << ", sum2 = "
				<< sum2 << std::endl;
	} catch (const std::overflow_error& e) {
		std::cout << "thread id = " << std::this_thread::get_id()
				<< ", e.what() = " << e.what() << std::endl;
	}
}
*/
