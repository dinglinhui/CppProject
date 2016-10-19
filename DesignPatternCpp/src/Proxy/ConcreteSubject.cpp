/*
 * ConcreteSubject.cpp
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#include "ConcreteSubject.h"

namespace DesignPattern {
namespace ProxyPattern {

ConcreteSubject::ConcreteSubject() {
}

ConcreteSubject::~ConcreteSubject() {
}

void ConcreteSubject::Request() {
	std::cout << "ConcreteSubject::Request" << std::endl;
}

} /* namespace ProxyPattern */
} /* namespace DesignPattern */
