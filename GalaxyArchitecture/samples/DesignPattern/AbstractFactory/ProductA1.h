#pragma once

#include "AbstractProductA.h"
namespace DesignPattern {
namespace AbstractFactoryPattern {

//派生类ProductA1，继承自AbstractProductA，A类产品的一种实现
class ProductA1: public AbstractProductA {
public:
	ProductA1();
	virtual void operation();
	virtual ~ProductA1();
};

} /* namespace AbstractFactoryPattern */
} /* namespace DesignPattern */
