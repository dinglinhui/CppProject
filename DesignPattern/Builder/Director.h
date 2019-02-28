/*
 * Director.h
 *
 *  Created on: Nov 11, 2014
 *      Author: Kevin
 */

#pragma once

#include "Builder.h"
namespace DesignPattern {
namespace BuilderPattern {

class Director {
public:
	Director(Builder* pBuilder);
	~Director();

	void Construct();
	//void Construct(const std::string& buildPara);
private:
	Builder* m_pBuilder = nullptr;
};

} /* namespace BuilderPattern */
} /* namespace DesignPattern */
