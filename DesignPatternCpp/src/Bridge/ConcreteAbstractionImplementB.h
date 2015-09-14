/*
 * ConcreteAbstractionImplementB.h
 *
 *  Created on: Nov 11, 2014
 *      Author: dlh
 */

#pragma once

#include "AbstractionImplement.h"
namespace DesignPattern
{
namespace BridgePattern
{

class ConcreteAbstractionImplementB:public AbstractionImplement
{
public:
    ConcreteAbstractionImplementB();
    ~ConcreteAbstractionImplementB();
    void Operation();
};

} /* namespace BridgePattern */
} /* namespace DesignPattern */
