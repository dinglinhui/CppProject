/*
 * ConcreteDecoratorC.h
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
class ConcreteDecoratorC: public Decorator {
public:
	ConcreteDecoratorC(Component* com);
	~ConcreteDecoratorC();
	virtual void Operation();
	void AddBehavorC();
};

} /* namespace DecoratorPattern */
} /* namespace DesignPattern */
