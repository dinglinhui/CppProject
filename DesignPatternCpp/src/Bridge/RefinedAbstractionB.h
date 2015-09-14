/*
 * RefinedAbstractionB.h
 *
 *  Created on: Nov 11, 2014
 *      Author: dlh
 */

#pragma once

#include "Abstraction.h"
#include "AbstractionImplement.h"
namespace DesignPattern
{
namespace BridgePattern
{

class AbstractionImplement;
class RefinedAbstractionB: public Abstraction
{
public:
	RefinedAbstractionB(AbstractionImplement* imp);
	virtual void Operation();
	virtual ~RefinedAbstractionB();
private:
	AbstractionImplement* _imp = nullptr;
};

} /* namespace BridgePattern */
} /* namespace DesignPattern */
