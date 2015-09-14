#include "Adapter1.h"

namespace DesignPattern
{
namespace AdapterPattern
{

Adapter1::Adapter1() :
		_adaptee(new Adaptee)
{
}

Adapter1::Adapter1(Adaptee* _adaptee)
{
	this->_adaptee = _adaptee;
}

Adapter1::~Adapter1()
{
}

void Adapter1::Request()
{
	cout << "Adapter1::Request()" << endl;
	this->_adaptee->SpecificRequest();
	cout << "----------------------------" << endl;
}

} /* namespace AdapterPattern */
} /* namespace DesignPattern */
