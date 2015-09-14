#pragma once

namespace DesignPattern
{
namespace AdapterPattern
{

class Target
{
public:
	Target();
	virtual ~Target();
	virtual void Request();
};

} /* namespace AdapterPattern */
} /* namespace DesignPattern */
