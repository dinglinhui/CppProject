/*
 * SmartPtr2.cpp
 *
 *  Created on: Oct 11, 2014
 *      Author: xdinlin
 */

#include <iostream>
#include <memory>

class Woman;
class Man {
private:
	std::weak_ptr<Woman> _wife;
	//std::shared_ptr<Woman> _wife;
public:
	void marriage(std::shared_ptr<Woman> woman) {
		_wife = woman;
	}

	void doSomthing() {
		std::shared_ptr<Woman> wife = _wife.lock();
		if (wife != nullptr) {

		}
	}

	~Man() {
		std::cout << "kill man\n";
	}
};

class Woman {
private:
	//std::weak_ptr<Man> _husband;
	std::shared_ptr<Man> _husband;
public:
	void marriage(std::shared_ptr<Man> man) {
		_husband = man;
	}

	~Woman() {
		std::cout << "kill woman\n";
	}
};

#if 0
int main(int argc, char** argv)
{
	std::shared_ptr<Man> man(new Man());
	std::shared_ptr<Woman> woman(new Woman());
	if (man && woman)
	{
		man->marriage(woman);
		woman->marriage(man);
	}
	return 0;
}
#endif

