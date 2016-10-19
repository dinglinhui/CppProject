/*
 * FlyweightFactory.cpp
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#include "FlyweightFactory.h"
#include "ConcreteFlyweight.h"

namespace DesignPattern {
namespace FlyweightPattern {

FlyweightFactory::FlyweightFactory() {
}

FlyweightFactory::~FlyweightFactory() {
}

Flyweight* FlyweightFactory::GetFlyweight(std::string key) {
	std::vector<Flyweight*>::iterator iter = this->m_vecFly.begin();
	for (; iter != this->m_vecFly.end(); iter++) {
		if ((*iter)->GetIntrinsicState() == key) {
			return *iter;
		}
	}
	Flyweight* fly = new ConcreteFlyweight(key);
	this->m_vecFly.push_back(fly);
	return fly;
}

void FlyweightFactory::GetFlyweightCount() {
	std::cout << this->m_vecFly.size() << std::endl;
}

} /* namespace FlyweightPattern */
} /* namespace DesignPattern */
