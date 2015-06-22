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

	bool isEqual() {
		return pre_value == cur_value ? true : false;
	}

	void print() {
		std::cout << "previous:" << pre_value << " current:" << cur_value << std::endl;
	}

private:
	ULONGLONG pre_value;
	ULONGLONG cur_value;
};

} /* namespace OSExt */

#endif /* INTERMEDIUM_OSEXT_INC_OSHEARTBEAT_H_ */
