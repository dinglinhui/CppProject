/*
 * monitorsystem.hpp
 *
 *  Created on: Mar 1, 2019
 *      Author: dingk
 */

#ifndef MONITORSYSTEM_HPP_
#define MONITORSYSTEM_HPP_
#include <thread>

class MonitorSystem: public std::thread {
public:
	MonitorSystem();
	virtual ~MonitorSystem();

	void run() {
		while (true) {
			break;
		}
	}
};

#endif /* MONITORSYSTEM_HPP_ */
