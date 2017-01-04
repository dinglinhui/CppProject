#include "Decorator.h"
#include <iostream>

namespace DesignPattern {
namespace DecoratorPattern {

Decorator::Decorator(Component* com) {
	this->_com = com;
}

void Decorator::SetComponent(Component* com) {
	this->_com = com;
}

Decorator::~Decorator() {
	std::cout << "~Decorator" << std::endl;
	delete this->_com;
	this->_com = nullptr;
}

void Decorator::Operation() {
}

} /* namespace DecoratorPattern */
} /* namespace DesignPattern */
