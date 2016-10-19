/*
 * ConcreteDecoratorD.cpp
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#include "ConcreteDecoratorD.h"

namespace DesignPattern {
namespace DecoratorPattern {

ConcreteDecoratorD::ConcreteDecoratorD(Component* com) :
		Decorator(com) {
}

ConcreteDecoratorD::~ConcreteDecoratorD() {
	std::cout << "~ConcreteDecoratorD" << std::endl;
}

void ConcreteDecoratorD::Operation() {
	this->_com->Operation();
	this->AddBehavorD();
}

void ConcreteDecoratorD::AddBehavorD() {
	std::cout << "ConcreteDecoratorD::AddBehavorD" << std::endl;
}

} /* namespace DecoratorPattern */
} /* namespace DesignPattern */
