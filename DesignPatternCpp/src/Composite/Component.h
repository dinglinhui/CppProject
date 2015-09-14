/*
 * Component.h
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#pragma once

#include <iostream>

namespace DesignPattern
{
namespace CompositePattern
{

class Component
{
public:
	virtual void Operation()=0;

	virtual void Add(Component*);
	virtual void Remove(Component*);
	virtual Component* GetChild(int index);
	virtual ~Component()
	{
	}

protected:
	Component()
	{
	}
};

} /* namespace CompositePattern */
} /* namespace DesignPattern */
