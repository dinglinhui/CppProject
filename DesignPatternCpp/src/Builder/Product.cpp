/*
 * Product.cpp
 *
 *  Created on: Nov 11, 2014
 *      Author: Kevin
 */

#include "Product.h"

namespace DesignPattern {
namespace BuilderPattern {

Product::~Product() {
}

Product::Product() {
}

void Product::setPartA(const std::string& s) {
	this->m_partA = s;
}

void Product::setPartB(const std::string& s) {
	this->m_partB = s;
}

void Product::setPartC(const std::string& s) {
	this->m_partC = s;
}

} /* namespace BuilderPattern */
} /* namespace DesignPattern */
