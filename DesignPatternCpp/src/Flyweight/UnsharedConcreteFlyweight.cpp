/*
 * UnsharedConcreteFlyweight.cpp
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#include "UnsharedConcreteFlyweight.h"

namespace DesignPattern
{
namespace FlyweightPattern
{

UnsharedConcreteFlyweight::UnsharedConcreteFlyweight(std::string intrinsicState) :
		Flyweight(intrinsicState)
{
}

UnsharedConcreteFlyweight::~UnsharedConcreteFlyweight()
{
}

void UnsharedConcreteFlyweight::Operation(const std::string& extrinsicState)
{
	std::cout << "extrinsicState" << std::endl;
}

} /* namespace FlyweightPattern */
} /* namespace DesignPattern */
