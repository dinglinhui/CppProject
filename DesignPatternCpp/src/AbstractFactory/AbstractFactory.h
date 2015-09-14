#pragma once

#include "AbstractProductA.h"
#include "AbstractProductB.h"
namespace DesignPattern {
namespace AbstractFactoryPattern {

class AbstractProductA;
class AbstractProductB;
class AbstractFactory {
public:
	virtual ~AbstractFactory()=0;
	virtual AbstractProductA* CreateProductA()=0;
	virtual AbstractProductB* CreateProductB()=0;
protected:
	AbstractFactory();
};

} /* namespace AbstractFactoryPattern */
} /* namespace DesignPattern */
