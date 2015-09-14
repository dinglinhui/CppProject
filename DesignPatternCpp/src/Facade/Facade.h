#pragma once

#include <iostream>
#include "Subsystem1.h"
#include "Subsystem2.h"
namespace DesignPattern
{
namespace FacadePattern
{

class Subsystem1;
class Subsystem2;
class Facade
{
public:
	Facade();
	~Facade();
	void OperationWrapper();
private:
	Subsystem1* _subsys1 = nullptr;
	Subsystem2* _subsys2 = nullptr;
};

} /* namespace FacadePattern */
} /* namespace DesignPattern */
