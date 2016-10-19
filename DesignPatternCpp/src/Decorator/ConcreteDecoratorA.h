/*
 * ConcreteDecoratorA.h
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#pragma once

#include "Component.h"
#include "Decorator.h"
namespace DesignPattern {
namespace DecoratorPattern {

class Component;
class ConcreteDecoratorA: public Decorator {
public:
	ConcreteDecoratorA(Component* com);
	~ConcreteDecoratorA();
	virtual void Operation();
	void AddBehavorA();
};
} /* namespace DecoratorPattern */
} /* namespace DesignPattern */
