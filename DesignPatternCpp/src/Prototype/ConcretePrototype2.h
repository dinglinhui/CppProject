/*
 * ConcretePrototype2.h
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
class ConcretePrototype2: public Prototype
{
public:
	ConcretePrototype2();
	~ConcretePrototype2();
	ConcretePrototype2(const ConcretePrototype2&);
	virtual Prototype* Clone() const;
};

} /* namespace PrototypePattern */
} /* namespace DesignPattern */
