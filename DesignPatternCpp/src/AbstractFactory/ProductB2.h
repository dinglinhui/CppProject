#pragma once

#include "AbstractProductB.h"
namespace DesignPattern {
namespace AbstractFactoryPattern {

//派生类ProductB2，继承自AbstractProductB，B类产品的另一种实现
class ProductB2: public AbstractProductB {
public:
	ProductB2();
	virtual void operation();
	virtual ~ProductB2();
};

} /* namespace AbstractFactoryPattern */
} /* namespace DesignPattern */
