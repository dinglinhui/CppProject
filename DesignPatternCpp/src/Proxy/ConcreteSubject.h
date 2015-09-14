/*
 * ConcreteSubject.h
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#pragma once

#include "Subject.h"
namespace DesignPattern
{
namespace ProxyPattern
{

class ConcreteSubject: public Subject
{
public:
	ConcreteSubject();
	~ConcreteSubject();

	virtual void Request();
};

} /* namespace ProxyPattern */
} /* namespace DesignPattern */
