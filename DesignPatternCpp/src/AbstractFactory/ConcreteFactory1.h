#pragma once

#include "AbstractFactory.h"
#include "AbstractProductA.h"
#include "AbstractProductA.h"
#include "ProductA1.h"
#include "ProductB1.h"
namespace DesignPattern {
namespace AbstractFactoryPattern {

class AbstractProductA;
class AbstractProductB;
class ConcreteFactory1: public AbstractFactory {
public:
	ConcreteFactory1();
	~ConcreteFactory1();

	virtual AbstractProductA* CreateProductA() override;
	virtual AbstractProductB* CreateProductB() override;
};

} /* namespace AbstractFactoryPattern */
} /* namespace DesignPattern */
