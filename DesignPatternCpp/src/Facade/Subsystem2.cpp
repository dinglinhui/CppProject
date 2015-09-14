/*
 * Subsystem2.cpp
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#include <iostream>
#include "Subsystem2.h"

namespace DesignPattern
{
namespace FacadePattern
{

Subsystem2::Subsystem2()
{
}

Subsystem2::~Subsystem2()
{
}

void Subsystem2::Operation()
{
	std::cout << "Subsystem2::Operation" << std::endl;
}

} /* namespace FacadePattern */
} /* namespace DesignPattern */
