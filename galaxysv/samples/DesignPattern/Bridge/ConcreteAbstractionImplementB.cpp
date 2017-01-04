/*
 * ConcreteAbstractionImplementB.cpp
 *
 *  Created on: Nov 11, 2014
 *      Author: dlh
 */

#include "ConcreteAbstractionImplementB.h"

namespace DesignPattern {
namespace BridgePattern {

ConcreteAbstractionImplementB::ConcreteAbstractionImplementB() {
}

ConcreteAbstractionImplementB::~ConcreteAbstractionImplementB() {
}

void ConcreteAbstractionImplementB::Operation() {
	std::cout << "ConcreteAbstractionImplementB Operation" << std::endl;
}

} /* namespace BridgePattern */
} /* namespace DesignPattern */
