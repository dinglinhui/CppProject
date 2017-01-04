/*
 * ConcreteBuilder1.h
 *
 *  Created on: Nov 11, 2014
 *      Author: Kevin
 */

#pragma once

#include "Builder.h"
#include "Product.h"
namespace DesignPattern {
namespace BuilderPattern {

class ConcreteBuilder1: public Builder {
public:
	ConcreteBuilder1();
	~ConcreteBuilder1();

	virtual void BuildPartA();
	virtual void BuildPartB();
	virtual void BuildPartC();
	virtual Product* GetProduct();
private:
	Product* m_pProduct = nullptr;
};

} /* namespace BuilderPattern */
} /* namespace DesignPattern */
