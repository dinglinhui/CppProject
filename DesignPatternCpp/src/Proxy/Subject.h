/*
 * Subject.h
 *
 *  Created on: Nov 12, 2014
 *      Author: dlh
 */

#pragma once

#include <iostream>
namespace DesignPattern
{
namespace ProxyPattern
{

class Subject
{
protected:
	Subject();
public:
	virtual ~Subject();
	virtual void Request()=0;
};

} /* namespace ProxyPattern */
} /* namespace DesignPattern */
