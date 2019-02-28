#pragma once
#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include <future>
#include <mutex>
#include <vector>

namespace multithreading {
class Guard {
public:
	Guard(std::thread& t) :
			thread(t) {
	}

	~Guard() {
		if (thread.joinable()) {
			thread.join();
		}
	}

	Guard(Guard& other) = delete;
	Guard& operator=(const Guard& rhs) = delete;

private:
	std::thread& thread;
};

class Car {
public:
	Car() :
			hood(true), electricity(true), exhaust(true), engine(true) {
	}

	void remove_the_hood() {
		hood = false;
	}

	void add_the_hood() {
		hood = true;
	}

	void disconnect_electricity() {
		electricity = false;
	}

	void connect_electricity() {
		electricity = true;
	}

	void remove_exhaust_system() {
		exhaust = false;
	}

	void add_exhaust_system() {
		exhaust = true;
	}

	bool is_engine_ok() const {
		return engine;
	}

private:
	bool hood;
	bool electricity;
	bool exhaust;
	bool engine;
};

void make_break(int milisec) {
	std::this_thread::sleep_for(std::chrono::milliseconds(milisec));
}

void mechanic(Car& car, std::promise<bool> engine_ok) {
	car.remove_the_hood();
	car.disconnect_electricity();
	car.remove_exhaust_system();

	std::cout << "Mechanic: took a car apart." << std::endl;

	engine_ok.set_value(car.is_engine_ok());

	std::cout << "Mechanic: engine is ok." << std::endl;

	if (car.is_engine_ok()) {
		make_break(10);
		car.add_exhaust_system();
		car.connect_electricity();
		car.add_the_hood();

		std::cout << "Mechanic: put car back together." << std::endl;
	}
}

void manager() {
	Car car;

	std::cout << "Manager: I would need to know if the car engine is ok."
			<< std::endl;

	std::promise<bool> promise;
	std::future<bool> answer = promise.get_future();

	std::thread thread(mechanic, std::ref(car), std::move(promise));
	thread.detach();

	if (answer.get()) {
		std::cout << "Manager: ensures the client that the engine is ok.\n"
				<< "Manager: negotiates the contract for selling the car."
				<< std::endl;
	} else {
		std::cout << "Manager: buys a new engine." << std::endl;
	}

	make_break(10);
}

void temperature(std::promise<int> answer) {
	std::cout << "Husband: Hm, is the weather " << "forcast in the newspaper?\n"
			<< "         Eh, we don't " << "have a newspaper at home..."
			<< std::endl;
	make_break(2);

	std::cout << "Husband: I will look it up on the internet!" << std::endl;
	make_break(2);

	std::cout << "Husband: Here it is, " << "it says tomorrow will be 40."
			<< std::endl;

	answer.set_value(40);
}

void test_future() {
	std::cout << "Wife:    Tomorrow, we are going on a picnic.\n"
			<< "         What will be the weather...\n"
			<< "         \"What will be the " << "temperature tomorrow?\""
			<< std::endl;

	auto temperature =
			[] {
				std::cout << "Husband: Hm, is the weather " << "forecast in the newspaper?\n"
				<< "         Eh, we don't " << "have a newspaper at home..." << std::endl;
				make_break(2);
				std::cout << "Husband: I will look it up on the internet!" << std::endl;
				make_break(2);
				std::cout << "Husband: Here it is, " << "it says tomorrow will be 40." << std::endl;
				return 40;
			};

	std::future<int> answer = std::async(temperature);
	make_break(2);
	std::cout << "Wife:    I should pack for tomorrow." << std::endl;
	make_break(2);
	std::cout
			<< "Wife:    Hopefully my husband can figure out the weather soon."
			<< std::endl;
	size_t temp = answer.get();
	std::cout << "Wife:    Finally, tomorrow will be " << temp << "... Em...\n"
			<< "         \"In which units is the answer?\"" << std::endl;
}

void test_promise() {
	std::cout << "Wife:    Tomorrow, we are going on a picnic.\n"
			<< "         What will be the weather...\n"
			<< "         \"What will be the " << "temperature tomorrow?\""
			<< std::endl;
	std::promise<int> promise;
	std::future<int> answer = promise.get_future();
	std::thread thread(temperature, std::move(promise));
	thread.detach();

	make_break(2);
	std::cout << "Wife:    I should pack for tomorrow." << std::endl;
	make_break(2);
	std::cout
			<< "Wife:    Hopefully my husband can figure out the weather soon."
			<< std::endl;
	size_t temp = answer.get();
	std::cout << "Wife:    Finally, tomorrow will be " << temp << "... Em...\n"
			<< "         \"In which units is the answer?\"" << std::endl;
}

void test_thread() {
	{
		std::thread thread_1([]() {
			for (int i = 0; i != 4; i++) {
				std::cout << "Function 1 i = " << i << std::endl;
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}

		});

		std::thread thread_2([]() {
			for (int j = 0; j != 4; j++) {
				std::cout << "Function 2 j = " << j << std::endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
			}
		});
		thread_1.join();
		thread_2.join();
	}

	{
		auto fun = [](int n, const std::string& msg) {
			for (int i = 0; i != n; i++) {
				std::cout << msg << std::endl;
			}
		};
		std::string msg1 = "Today is a beautiful day.";
		std::thread t1(fun, 6, std::ref(msg1));

		std::string msg2 = "And sun is shining.";
		std::thread t2(fun, 6, std::ref(msg2));

		t1.join();
		t2.join();
	}

	{
		std::thread t([] {
			std::cout << "I'm inside function." << std::endl;
		});
		Guard guard(t);
	}

	{
		std::mutex increment;
		int n = 0;
		std::vector<int> numbers1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		std::vector<int> numbers2 = { 11, 12, 13, 14, 15, 16, 17, 18 };
		auto countEven =
				[&increment](const std::vector<int>& numbers, int& numEven) {
					for (const auto n : numbers) {
						if (n % 2 == 0) {
							increment.lock();
							numEven++;
							increment.unlock();
						}
					}
				};
		std::thread t1(countEven, std::ref(numbers1), std::ref(n));
		std::thread t2(countEven, std::ref(numbers2), std::ref(n));

		t1.join();
		t2.join();

		std::cout << "Number of even numbers is: " << n << std::endl;
	}
}
} /* namespace thread */
