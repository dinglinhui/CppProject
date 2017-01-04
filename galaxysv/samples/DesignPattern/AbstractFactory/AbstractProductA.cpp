#include <iostream>
#include "AbstractProductA.h"

namespace DesignPattern {
namespace AbstractFactoryPattern {

//AbstractProductA
AbstractProductA::AbstractProductA() {
	std::cout << "AbstractProductA..." << std::endl;
}

AbstractProductA::~AbstractProductA() {
	std::cout << "~AbstractProductA..." << std::endl;
}

} /* namespace AbstractFactoryPattern */
} /* namespace DesignPattern */
