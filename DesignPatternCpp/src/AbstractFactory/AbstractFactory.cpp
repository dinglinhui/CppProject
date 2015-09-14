#include <iostream>
#include "AbstractFactory.h"
#include "AbstractProductA.h"

namespace DesignPattern {
namespace AbstractFactoryPattern {

AbstractFactory::AbstractFactory() {
	std::cout << "AbstractFactory..." << std::endl;
}

AbstractFactory::~AbstractFactory() {
	std::cout << "~AbstractFactory..." << std::endl;
}

} /* namespace AbstractFactoryPattern */
} /* namespace DesignPattern */

