/*
 * Character.h
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#pragma once

#include <iostream>
namespace DesignPattern {
namespace FlyweightPattern {

class Character {
protected:
	Character(char c);

public:
	virtual ~Character();

	virtual void Display(int width, int height, int ascent, int descent,
			int pointSize)=0;
	virtual char GetSymbol()=0;

protected:
	char symbol;

	int width = 0;
	int height = 0;
	int ascent = 0;
	int descent = 0;
	int pointSize = 0;
};

} /* namespace FlyweightPattern */
} /* namespace DesignPattern */
