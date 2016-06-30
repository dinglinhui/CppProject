/*
#include <iostream>
#include <future>
#include <thread>
#include <cstdint>
#include <stdexcept>
#include <limits>

std::uint32_t add(std::uint32_t a, std::uint32_t b) {
	std::cout << "thread id = " << std::this_thread::get_id() << ", a = " << a
			<< ", b = " << b << std::endl;
	if (std::numeric_limits<std::uint32_t>::max() - a < b) {
		throw std::overflow_error("overflow_error");
	}
	return a + b;
}

int main() {
	// 使用std::async启动异步操作并返回std::future对象
	std::future<std::uint32_t> f1 = std::async(std::launch::async, add, 1ul,
			2ul);
	// 通过std::future::get等待异步操作完成并取得返回值
	std::uint32_t sum1 = f1.get();
	std::cout << "thread id = " << std::this_thread::get_id() << ", sum1 = "
			<< sum1 << std::endl;
	// 4000000000ul + 4000000000ul会抛出异常，异常会被延迟到std::future::get或std::future::wait时抛出
	std::future<std::uint32_t> f2 = std::async(std::launch::async, add,
			4000000000ul, 4000000000ul);
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
