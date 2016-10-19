/*
 * UnsharedConcreteFlyweight.h
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#pragma once

#include "Flyweight.h"

namespace DesignPattern {
namespace FlyweightPattern {

class UnsharedConcreteFlyweight: public Flyweight {
public:
	virtual void Operation(const std::string& extrinsicState);
	UnsharedConcreteFlyweight(std::string intrinsicState);
	~UnsharedConcreteFlyweight();
};

} /* namespace FlyweightPattern */
} /* namespace DesignPattern */
