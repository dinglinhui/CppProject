#include "ConcreteFactory1.h"

namespace DesignPattern {
namespace AbstractFactoryPattern {

ConcreteFactory1::ConcreteFactory1() {
	std::cout << "ConcreteFactory1..." << std::endl;
}

ConcreteFactory1::~ConcreteFactory1() {
	std::cout << "~ConcreteFactory1..." << std::endl;
}

AbstractProductA* ConcreteFactory1::CreateProductA() {
	return new ProductA1 { };
}

AbstractProductB* ConcreteFactory1::CreateProductB() {
	return new ProductB1 { };
}

} /* namespace AbstractFactoryPattern */
} /* namespace DesignPattern */
