/*
 * Product.h
 *
 *  Created on: Nov 11, 2014
 *      Author: Kevin
 */

#pragma once
#include <iostream>
#include <string>
//#include <vector>

namespace DesignPattern
{
namespace BuilderPattern
{

class Product
{
public:
    Product();
    ~Product();

    void setPartA(const std::string& s);
    void setPartB(const std::string& s);
    void setPartC(const std::string& s);

private:
    std::string m_partA;
    std::string m_partB;
    std::string m_partC;
};

} /* namespace BuilderPattern */
} /* namespace DesignPattern */
