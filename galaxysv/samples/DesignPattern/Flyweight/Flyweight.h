#pragma once

#include <string>
#include <vector>
#include <iostream>

namespace DesignPattern {
namespace FlyweightPattern {

class Flyweight {
protected:
	Flyweight(std::string intrinsicState);

public:
	virtual ~Flyweight();

	virtual void Operation(const std::string& extrinsicState)=0;
	std::string GetIntrinsicState();

private:
	std::string _intrinsicState;
};

} /* namespace FlyweightPattern */
} /* namespace DesignPattern */
