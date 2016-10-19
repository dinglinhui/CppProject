#pragma once

#include "AbstractProductB.h"

namespace DesignPattern {
namespace AbstractFactoryPattern {

//派生类ProductB1，继承自AbstractProductB，B类产品的一种实现
class ProductB1: public AbstractProductB {
public:
	ProductB1();
	virtual void operation();
	virtual ~ProductB1();
};

} /* namespace AbstractFactoryPattern */
} /* namespace DesignPattern */

