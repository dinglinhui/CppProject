/*
 * DecoratorOnlyOne.h
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#pragma once

#include "Component.h"
namespace DesignPattern
{
namespace DecoratorPattern
{

class Component;
class DecoratorOnlyOne: public Component
{
public:
	DecoratorOnlyOne(Component* com);
	~DecoratorOnlyOne();
	virtual void Operation();
	void AddBehavor();
private:
	Component* _com = nullptr;
};

} /* namespace DecoratorPattern */
} /* namespace DesignPattern */
