#pragma once

namespace DesignPattern
{
namespace CommandPattern
{
class Command
{
public:
	virtual ~Command();
	virtual void Execute()=0;

protected:
	Command();
};

} /* namespace CommandPattern */
} /* namespace DesignPattern */
