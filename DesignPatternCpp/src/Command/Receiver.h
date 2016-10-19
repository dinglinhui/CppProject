/*
 * Receiver.h
 *
 *  Created on: Nov 10, 2014
 *      Author: dlh
 */

#pragma once
#include <iostream>

namespace DesignPattern {
namespace CommandPattern {
class Receiver {
public:
	Receiver();
	~Receiver();

	void Action();
};

} /* namespace CommandPattern */
} /* namespace DesignPattern */
