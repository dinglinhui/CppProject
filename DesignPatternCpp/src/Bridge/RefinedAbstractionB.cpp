/*
 * RefinedAbstractionB.cpp
 *
 *  Created on: Nov 11, 2014
 *      Author: dlh
 */

#include "RefinedAbstractionB.h"

namespace DesignPattern
{
namespace BridgePattern
{

RefinedAbstractionB::RefinedAbstractionB(AbstractionImplement* imp)
{
	this->_imp = imp;
}

RefinedAbstractionB::~RefinedAbstractionB()
{
	delete this->_imp, this->_imp = nullptr;
}

void RefinedAbstractionB::Operation()
{
	std::cout << "RefinedAbstractionB::Operation" << std::endl;
	this->_imp->Operation();
}


} /* namespace BridgePattern */
} /* namespace DesignPattern */
