/*
 * ConcreteAbstractionImplementA.h
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

class ConcreteAbstractionImplementA:public AbstractionImplement
{
public:
    ConcreteAbstractionImplementA();
    ~ConcreteAbstractionImplementA();
    void Operation();
};

} /* namespace BridgePattern */
} /* namespace DesignPattern */
