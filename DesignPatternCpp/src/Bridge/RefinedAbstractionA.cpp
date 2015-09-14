/*
 * RefinedAbstractionA.cpp
 *
 *  Created on: Nov 11, 2014
 *      Author: dlh
 */

#include "RefinedAbstractionA.h"

namespace DesignPattern
{
namespace BridgePattern
{

RefinedAbstractionA::RefinedAbstractionA(AbstractionImplement* imp)
{
	this->_imp = imp;
}

RefinedAbstractionA::~RefinedAbstractionA()
{
	delete this->_imp;
	this->_imp = nullptr;
}

void RefinedAbstractionA::Operation()
{
	std::cout << "RefinedAbstractionA::Operation" << std::endl;
	this->_imp->Operation();
}

} /* namespace BridgePattern */
} /* namespace DesignPattern */
