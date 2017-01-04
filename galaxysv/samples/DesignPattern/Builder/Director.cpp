/*
 * Director.cpp
 *
 *  Created on: Nov 11, 2014
 *      Author: Kevin
 */

#include "Director.h"

namespace DesignPattern {
namespace BuilderPattern {

Director::Director(Builder* pBuilder) {
	this->m_pBuilder = pBuilder;
}

void Director::Construct() {
	this->m_pBuilder->BuildPartA();
	this->m_pBuilder->BuildPartB();
	this->m_pBuilder->BuildPartC();
}

Director::~Director() {
	delete this->m_pBuilder;
	this->m_pBuilder = nullptr;
}

} /* namespace BuilderPattern */
} /* namespace DesignPattern */
