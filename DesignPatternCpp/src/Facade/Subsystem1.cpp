/*
 * Subsystem1.cpp
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#include <iostream>
#include "Subsystem1.h"

namespace DesignPattern
{
namespace FacadePattern
{

Subsystem1::Subsystem1()
{
}

Subsystem1::~Subsystem1()
{
}

void Subsystem1::Operation()
{
	std::cout << "Subsystem1::Operation" << std::endl;
}

} /* namespace FacadePattern */
} /* namespace DesignPattern */
