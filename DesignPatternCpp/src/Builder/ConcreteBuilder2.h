/*
 * ConcreteBuilder2.h
 *
 *  Created on: Nov 11, 2014
 *      Author: Kevin
 */

#pragma once

#include "Builder.h"
#include "Product.h"

namespace DesignPattern {
namespace BuilderPattern {
class Product;
class ConcreteBuilder2: public Builder {
public:
	ConcreteBuilder2();
	~ConcreteBuilder2();

	virtual void BuildPartA();
	virtual void BuildPartB();
	virtual void BuildPartC();
	virtual Product* GetProduct();
private:
	Product* m_pProduct = nullptr;
};

} /* namespace BuilderPattern */
} /* namespace DesignPattern */
