/*
 * OSHeartbeat.h
 *
 *  Created on: Jun 22, 2015
 *      Author: dinglinhui
 */

#ifndef OSHEARTBEAT_H_
#define OSHEARTBEAT_H_

#include <iostream>
#include "OSDefs.h"

namespace osext {

class OSHeartbeat {
public:
	OSHeartbeat();
	virtual ~OSHeartbeat();

	OSHeartbeat& operator++(); // prefix
	const OSHeartbeat operator++(int); // suffix

	OSHeartbeat& operator--(); // prefix
	const OSHeartbeat operator--(int); // suffix

	friend std::ostream &operator<<(std::ostream &os, const OSHeartbeat &heartbeat);
	friend std::istream &operator>>(std::istream &is, OSHeartbeat &heartbeat);

	bool isEqual() {
		return pre_value == cur_value ? true : false;
	}

	void print() {
		std::cout << *this;
	}

private:
	ULONGLONG pre_value;
	ULONGLONG cur_value;
};

} /* namespace OSExt */

#endif /* INTERMEDIUM_OSEXT_INC_OSHEARTBEAT_H_ */
