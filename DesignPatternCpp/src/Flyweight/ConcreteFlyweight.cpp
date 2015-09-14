/*
 * ConcreteFlyweight.cpp
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#include "ConcreteFlyweight.h"

namespace DesignPattern
{
namespace FlyweightPattern
{

ConcreteFlyweight::ConcreteFlyweight(std::string intrinsicState) :
		Flyweight(intrinsicState)
{
}

ConcreteFlyweight::~ConcreteFlyweight()
{
}

void ConcreteFlyweight::Operation(const std::string& extrinsicState)
{
	std::cout << this->GetIntrinsicState() << std::endl;
	std::cout << extrinsicState << std::endl;
}

} /* namespace FlyweightPattern */
} /* namespace DesignPattern */
