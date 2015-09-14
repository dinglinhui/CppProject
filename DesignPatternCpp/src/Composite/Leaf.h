/*
 * Leaf.h
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#pragma once

#include "Component.h"
namespace DesignPattern
{
namespace CompositePattern
{

class Leaf:public Component
{
public:
    virtual void Operation();
    Leaf();
    ~Leaf();
};


} /* namespace CompositePattern */
} /* namespace DesignPattern */
