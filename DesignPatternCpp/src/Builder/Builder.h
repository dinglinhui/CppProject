#pragma once

#include "Product.h"

namespace DesignPattern {
namespace BuilderPattern {

class Builder {
public:
	Builder();
	virtual ~Builder();

	virtual void BuildPartA()=0;
	virtual void BuildPartB()=0;
	virtual void BuildPartC()=0;
	virtual Product* GetProduct()=0;
};

} /* namespace BuilderPattern */
} /* namespace DesignPattern */
