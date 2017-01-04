/*
 * RefinedAbstractionA.h
 *
 *  Created on: Nov 11, 2014
 *      Author: dlh
 */

#pragma once

#include "Abstraction.h"
#include "AbstractionImplement.h"
namespace DesignPattern {
namespace BridgePattern {

class AbstractionImplement;
class RefinedAbstractionA: public Abstraction {
public:
	RefinedAbstractionA(AbstractionImplement* imp);
	virtual void Operation();
	virtual ~RefinedAbstractionA();
private:
	AbstractionImplement* _imp = nullptr;
};

} /* namespace BridgePattern */
} /* namespace DesignPattern */
