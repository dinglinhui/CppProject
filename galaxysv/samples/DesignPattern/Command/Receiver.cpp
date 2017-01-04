/*
 * Receiver.cpp
 *
 *  Created on: Nov 10, 2014
 *      Author: dlh
 */

#include "Receiver.h"

namespace DesignPattern {
namespace CommandPattern {
Receiver::Receiver() {
}

Receiver::~Receiver() {
}

void Receiver::Action() {
	std::cout << "Receiver::Action" << std::endl;
}

} /* namespace CommandPattern */
} /* namespace DesignPattern */
