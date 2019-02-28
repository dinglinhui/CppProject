/*
 * ConcretePrototype2.cpp
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#include "ConcretePrototype2.h"

namespace DesignPattern {
namespace PrototypePattern {

ConcretePrototype2::ConcretePrototype2() {
	std::cout << "ConcretePrototype2" << std::endl;
}

ConcretePrototype2::~ConcretePrototype2() {
	std::cout << "~ConcretePrototype2" << std::endl;
}

ConcretePrototype2::ConcretePrototype2(const ConcretePrototype2& cp) {
	std::cout << "ConcretePrototype2 copy" << std::endl;
}

Prototype* ConcretePrototype2::Clone() const {
	return new ConcretePrototype2(*this);
}

} /* namespace PrototypePattern */
} /* namespace DesignPattern */
