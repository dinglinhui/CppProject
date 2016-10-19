#include "TemplateMethod.h"
#include <iostream>

namespace DesignPattern {
namespace TemplateMethodPattern {

AbstractClass::AbstractClass() {
}

AbstractClass::~AbstractClass() {
}

void AbstractClass::TemplateMethod() {
	this->PrimitiveOperation1();
	this->PrimitiveOperation2();
}

ConcreteClassA::ConcreteClassA() {
}

ConcreteClassA::~ConcreteClassA() {
}

void ConcreteClassA::PrimitiveOperation1() {
	std::cout << "ConcreteClassA::PrimitiveOperation1" << std::endl;
}

void ConcreteClassA::PrimitiveOperation2() {
	std::cout << "ConcreteClassA::PrimitiveOperation2" << std::endl;
}

ConcreteClassB::ConcreteClassB() {
}

ConcreteClassB::~ConcreteClassB() {
}

void ConcreteClassB::PrimitiveOperation1() {
	std::cout << "ConcreteClassB::PrimitiveOperation1" << std::endl;
}

void ConcreteClassB::PrimitiveOperation2() {
	std::cout << "ConcreteClassB::PrimitiveOperation2" << std::endl;
}

} /* namespace TemplateMethodPattern */
} /* namespace DesignPattern */
