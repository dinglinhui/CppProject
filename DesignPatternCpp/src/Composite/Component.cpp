/*
 * Component.cpp
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#include "Component.h"

namespace DesignPattern
{
namespace CompositePattern
{

void Component::Add(Component* com)
{
	std::cout << "add" << std::endl;
}

void Component::Remove(Component* com)
{
}

void Component::Operation()
{
	std::cout << "Component::Operation" << std::endl;
}

Component* Component::GetChild(int index)
{
	return nullptr;
}

} /* namespace CompositePattern */
} /* namespace DesignPattern */
