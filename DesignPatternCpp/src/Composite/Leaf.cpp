/*
 * Leaf.cpp
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#include "Leaf.h"

namespace DesignPattern
{
namespace CompositePattern
{

Leaf::Leaf()
{
}

Leaf::~Leaf()
{
}

void Leaf::Operation()
{
	std::cout << "Leaf::Operation" << std::endl;
}

} /* namespace CompositePattern */
} /* namespace DesignPattern */
