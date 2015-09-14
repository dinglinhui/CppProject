#include "Proxy.h"
#include "ConcreteSubject.h"
#include <iostream>

namespace DesignPattern
{
namespace ProxyPattern
{

Proxy::Proxy() :
		_subject(nullptr)
{
}

Proxy::~Proxy()
{
}

void Proxy::DoSomething1()
{
	std::cout << "Proxy::DoSomething1" << std::endl;
}

void Proxy::DoSomething2()
{
	std::cout << "Proxy::DoSomething2" << std::endl;
}

void Proxy::Request()
{
	if (nullptr == this->_subject)
	{
		this->_subject = new ConcreteSubject();
	}

	this->DoSomething1();
	this->_subject->Request();
	this->DoSomething2();
}

} /* namespace ProxyPattern */
} /* namespace DesignPattern */
