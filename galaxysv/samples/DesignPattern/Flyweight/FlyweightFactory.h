/*
 * FlyweightFactory.h
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#pragma once

#include "Flyweight.h"
namespace DesignPattern {
namespace FlyweightPattern {

class Flyweight;
class FlyweightFactory {
public:
	FlyweightFactory();
	~FlyweightFactory();

	Flyweight* GetFlyweight(std::string key);
	void GetFlyweightCount();

private:
	std::vector<Flyweight*> m_vecFly;
};

} /* namespace FlyweightPattern */
} /* namespace DesignPattern */
