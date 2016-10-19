/*
 * CharacterA.cpp
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#include "CharacterA.h"

namespace DesignPattern {
namespace FlyweightPattern {

CharacterA::CharacterA(char c) :
		Character(c) {
}

CharacterA::~CharacterA() {
}

char CharacterA::GetSymbol() {
	return this->symbol;
}

void CharacterA::Display(int width, int height, int ascent, int descent,
		int pointSize) {
	this->ascent = ascent;
	this->descent = descent;
	this->height = height;
	this->pointSize = pointSize;
	this->width = width;

	std::cout << this->symbol << " " << this->ascent << " " << this->descent
			<< " " << this->height << " " << this->pointSize << " "
			<< this->width << std::endl;
}

} /* namespace FlyweightPattern */
} /* namespace DesignPattern */
