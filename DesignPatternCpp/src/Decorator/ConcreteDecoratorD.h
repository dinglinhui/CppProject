/*
 * ConcreteDecoratorD.h
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
class ConcreteDecoratorD: public Decorator {
public:
	ConcreteDecoratorD(Component* com);
	~ConcreteDecoratorD();
	virtual void Operation();
	void AddBehavorD();
};

} /* namespace DecoratorPattern */
} /* namespace DesignPattern */
