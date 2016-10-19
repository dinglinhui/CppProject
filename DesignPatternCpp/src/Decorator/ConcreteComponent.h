/*
 * ConcreteComponent.h
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#pragma once

#include "Component.h"
namespace DesignPattern {
namespace DecoratorPattern {

class ConcreteComponent: public Component {
public:
	ConcreteComponent();
	~ConcreteComponent();
	virtual void Operation();
};

} /* namespace DecoratorPattern */
} /* namespace DesignPattern */
