#pragma once

#include "Subject.h"
namespace DesignPattern
{
namespace ProxyPattern
{

class Subject;
class Proxy: public Subject
{
public:
	Proxy();
	~Proxy();
	void DoSomething1();
	virtual void Request();
	void DoSomething2();
private:
	Subject* _subject = nullptr;
};

} /* namespace ProxyPattern */
} /* namespace DesignPattern */
