/*
 * Character.cpp
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#include "Character.h"

namespace DesignPattern {
namespace FlyweightPattern {

Character::Character(char c) {
	this->symbol = c;
}

Character::~Character() {
}

} /* namespace FlyweightPattern */
} /* namespace DesignPattern */
