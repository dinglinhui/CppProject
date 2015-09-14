#pragma once

#include <iostream>
namespace DesignPattern
{
namespace BridgePattern
{

class Abstraction
{
public:
	virtual void Operation()=0;
	virtual ~Abstraction();
protected:
	Abstraction();
};

} /* namespace BridgePattern */
} /* namespace DesignPattern */
