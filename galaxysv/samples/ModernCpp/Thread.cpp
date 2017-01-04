// condition_variable example
#include <iostream>           // std::cout
#include <thread>             // std::thread
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable
#include <string>
#include <vector>
#include <functional>
#include <future>

class Foo {
public:
	Foo() :
			flag_ { 0 }, thread1_(std::bind(&Foo::threadFunc1, this)), thread2_(std::bind(&Foo::threadFunc2, this)) {
	}

	~Foo() {
		thread1_.join();
		thread2_.join();
	}

private:
	void threadFunc1() {
		std::unique_lock<std::mutex> ul { mutex_ };
		while (0 == flag_) {
			cond_.wait(ul);
		}
		std::cout << flag_ << std::endl;
	}

	void threadFunc2() {
		// wait 3s for test
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		std::unique_lock<std::mutex> ul { mutex_ };
		flag_ = 100;
		cond_.notify_one();
	}

	int flag_;
	std::mutex mutex_;
	std::condition_variable cond_;
	std::thread thread1_;
	std::thread thread2_;
};

///////////////////////////////////////////////////////////////////////////////
// Simple
///////////////////////////////////////////////////////////////////////////////
void foo1() {
}

void func1() {
	std::thread t(foo1); // Starts. Equal to CreateThread.
	t.join();  // Equal to WaitForSingleObject to the thread handle.
}

///////////////////////////////////////////////////////////////////////////////
void foo2(int x, int y) {
	// x = 4, y = 5.
	int z { x + y };
	std::cout << z << std::endl;
}

void func2() {
	std::thread t(foo2, 4, 5); // Acceptable.
	t.join();
}

///////////////////////////////////////////////////////////////////////////////
// MUTEX
///////////////////////////////////////////////////////////////////////////////
std::mutex m3 { };
int j3 { 0 };
void foo3() {
	m3.lock();
	j3++;
	m3.unlock();
}
void func3() {
	std::thread t1(foo3);
	std::thread t2(foo3);
	t1.join();
	t2.join();
	// j3 = 2;
}

///////////////////////////////////////////////////////////////////////////////
std::recursive_mutex m4 { };
int j4 { 0 };
void func4() {
	m4.lock();
	m4.lock(); // now valid
	j4++;
	m4.unlock();
	m4.unlock(); // don't forget!
}

///////////////////////////////////////////////////////////////////////////////
// Variables
///////////////////////////////////////////////////////////////////////////////
std::condition_variable c { };
std::mutex mu { };
// We use a mutex rather than a recursive_mutex 
// because the lock has to be acquired only and exactly once.
void foo5() {
	std::unique_lock<std::mutex> lock { mu }; // Lock the mutex
	c.notify_one(); // WakeConditionVariable. It also releases the unique lock 
}

void func5() {
	std::unique_lock<std::mutex> lock { mu }; // Lock the mutex
	std::thread t1(foo5);
	c.wait(lock); // Equal to SleepConditionVariableCS. This unlocks the mutex mu
	t1.join();
}

///////////////////////////////////////////////////////////////////////////////
// Future and Promises
///////////////////////////////////////////////////////////////////////////////
int GetMyAnswer() {
	return 10;
}

void func6() {
	std::future<int> GetAnAnswer = std::async(GetMyAnswer); // GetMyAnswer starts background execution
	int answer = GetAnAnswer.get(); // answer = 10; 
	// If GetMyAnswer has finished, this call returns immediately. 
	// If not, it waits for the thread to finish.
	std::cout << answer << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
std::promise<int> sex { };
void foo7() {
	// do stuff

	sex.set_value(1); // After this call, future::get() will return this value. 
	//	sex.set_exception(std::make_exception_ptr(std::runtime_error("TEST"))); // After this call, future::get() will throw this exception
}
void func7() {
	std::future<int> makesex = sex.get_future();
	std::thread t(foo7);

	// do stuff
	try {
		makesex.get();
		// hurray();
	} catch (...) {
		// She dumped us :(
	}
	t.join();
}

/////////////////////////////////////////////////////////////
std::mutex mtx { };
std::condition_variable cv { };
bool ready { false };

void print_id(int id) {
	std::unique_lock<std::mutex> lck { mtx };
	while (!ready)
		cv.wait(lck);
	// ...
	std::cout << "thread " << id << '\n';
}

void go() {
	std::unique_lock<std::mutex> lck { mtx };
	ready = true;
	cv.notify_all();
}

void mythread1() {
	while (true) {
		std::cout << "abc ";
		std::this_thread::sleep_for((std::chrono::milliseconds(100)));
	}
}

void mythread2() {
	while (true) {
		std::cout << "def ";
		std::this_thread::sleep_for((std::chrono::milliseconds(100)));
	}
}
//
//int main()
//{
//	/*func1();
//	func2();
//	func3();
//	func4();
//	func5();
//	func6();
//	func7();
//	*/
//
//	/*
//	///////////////////////////////////////////////////////////////////
//	std::thread foo(mythread1);
//	std::thread bar(mythread2);
//
//	std::cout << "Joinable after construction:\n" << std::boolalpha;
//	std::cout << "foo: " << foo.joinable() << '\n';
//	std::cout << "bar: " << bar.joinable() << '\n';
//
//	if (foo.joinable())
//		foo.join();
//
//	if (bar.joinable())
//		bar.join();
//
//	std::cout << "Joinable after joining:\n" << std::boolalpha;
//	std::cout << "foo: " << foo.joinable() << '\n';
//	std::cout << "bar: " << bar.joinable() << '\n';
//
//	///////////////////////////////////////////////////////////////////
//	std::thread threads[10];
//	// spawn 10 threads:
//	for (int i = 0; i<10; ++i)
//		threads[i] = std::thread(print_id, i);
//
//	std::cout << "10 threads ready to race...\n";
//
//	go();                       // go!
//
//	for (auto& thread : threads)
//		thread.join();*/
//
//	Foo f;
//
//	//system("pause");
//	return 0;
//}
