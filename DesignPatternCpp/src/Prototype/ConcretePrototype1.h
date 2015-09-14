/*
 * ConcretePrototype1.h
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#pragma once

#include "Prototype.h"
namespace DesignPattern
{
namespace PrototypePattern
{

class Prototype;
class ConcretePrototype1: public Prototype
{
public:
	ConcretePrototype1();
	~ConcretePrototype1();
	ConcretePrototype1(const ConcretePrototype1&);
	virtual Prototype* Clone() const;
};

} /* namespace PrototypePattern */
} /* namespace DesignPattern */
