#pragma once

#include "AbstractFactory.h"
#include "AbstractProductA.h"
#include "AbstractProductB.h"
#include "ProductA2.h"
#include "ProductB2.h"
namespace DesignPattern {
namespace AbstractFactoryPattern {

class AbstractProductA;
class AbstractProductB;
class ConcreteFactory2: public AbstractFactory {
public:
	ConcreteFactory2();
	~ConcreteFactory2();
	virtual AbstractProductA* CreateProductA() override;
	virtual AbstractProductB* CreateProductB() override;
};

} /* namespace AbstractFactoryPattern */
} /* namespace DesignPattern */
