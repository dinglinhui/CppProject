/*
 * CharacterA.h
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#pragma once

#include "Character.h"
namespace DesignPattern
{
namespace FlyweightPattern
{

class CharacterA:public Character
{
public:
    CharacterA(char c);
    virtual ~CharacterA();

    virtual void Display(int width,int height,int ascent,int descent,int pointSize);
    virtual char GetSymbol();
};

} /* namespace FlyweightPattern */
} /* namespace DesignPattern */
