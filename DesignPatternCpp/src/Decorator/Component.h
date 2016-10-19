/*
 * Component.h
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#pragma once

#include <iostream>
namespace DesignPattern {
namespace DecoratorPattern {

class Component {
public:
	virtual ~Component();
	virtual void Operation()=0;
protected:
	Component();
};

} /* namespace DecoratorPattern */
} /* namespace DesignPattern */
