/*
 * ConcreteFlyweight.h
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#pragma once

#include "Flyweight.h"
namespace DesignPattern
{
namespace FlyweightPattern
{

class ConcreteFlyweight: public Flyweight
{
public:
	ConcreteFlyweight(std::string intrinsicState);
	~ConcreteFlyweight();

	virtual void Operation(const std::string& extrinsicState);
};

} /* namespace FlyweightPattern */
} /* namespace DesignPattern */
