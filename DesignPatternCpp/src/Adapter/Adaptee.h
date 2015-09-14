#pragma once

#include <iostream>
using namespace std;

namespace DesignPattern
{
namespace AdapterPattern
{

class Adaptee
{
public:
	Adaptee();
	virtual ~Adaptee();
	void SpecificRequest();
};

} /* namespace AdapterPattern */
} /* namespace DesignPattern */
