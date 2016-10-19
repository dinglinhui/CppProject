#pragma once

#include "AbstractProductA.h"

namespace DesignPattern {
namespace AbstractFactoryPattern {

//派生类ProductA2，继承自AbstractProductA，A类产品的另一种实现
class ProductA2: public AbstractProductA {
public:
	ProductA2();
	virtual void operation();
	virtual ~ProductA2();
};
} /* namespace AbstractFactoryPattern */
} /* namespace DesignPattern */
