/*
 * ConcreteDecoratorB.h
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#pragma once

#include "Component.h"
#include "Decorator.h"
namespace DesignPattern
{
namespace DecoratorPattern
{

class Component;
class ConcreteDecoratorB: public Decorator
{
public:
	ConcreteDecoratorB(Component* com);
	~ConcreteDecoratorB();
	virtual void Operation();
	void AddBehavorB();
};

} /* namespace DecoratorPattern */
} /* namespace DesignPattern */
