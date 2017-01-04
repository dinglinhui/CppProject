/*
#include <iostream>
#include <thread>

void thread_proc() {
	std::cout << "thread id = " << std::this_thread::get_id() << std::endl;
}

struct functor {
	void operator()(int a, int b) {
		std::cout << a << "+" << b << "=" << a + b << std::endl;
	}
};

class C {
	int data_;
public:
	C(int data) :
			data_(data) {
	}
	void member_func() {
		std::cout << "this->data_ = " << this->data_ << std::endl;
	}
};

int main() {
//	#1
	std::thread td1(thread_proc); // 创建thread对象即创建了一个新的线程并执行
	td1.join(); // 等待线程执行结束

//	#2
	std::thread td2([](int a, int b) {
		std::cout << a << "+" << b << "=" << a + b << std::endl;
	}, 1, 2); // 使用Lambda表达式创建线程对象并传递参数1和2
	td2.join(); // 等待线程执行结束

//	#3
	std::thread td3(functor(), 1, 2); // 使用functor对象创建线程对象并传递参数1和2
	td3.join(); // 等待线程执行结束

//	#4
	C obj(10);
	std::thread td4(std::bind(&C::member_func, &obj));
	td4.join(); // 等待线程执行结束

//	#5
	std::thread td5([&obj]() {
		obj.member_func();
	});
	std::cout << "td.joinable() = " << std::boolalpha << td5.joinable()
			<< std::endl;
	td5.join(); // 等待线程执行结束
	std::cout << "td.joinable() = " << std::boolalpha << td5.joinable()
			<< std::endl;

//	#6
	std::thread td6([]() {});
	std::cout << "td.joinable() = " << std::boolalpha << td6.joinable()
			<< std::endl;
	td6.detach();
	std::cout << "td.joinable() = " << std::boolalpha << td6.joinable()
			<< std::endl;
}
*/
