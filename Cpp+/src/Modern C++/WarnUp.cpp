/*
* WarnUp.cpp
*
*  Created on: Oct 13, 2014
*      Author: xdinlin
*/
#include <iostream>

int(*(*pf())())() { return nullptr; }

auto pf1() -> auto (*)() -> int(*)()
{
	return nullptr;
}

auto fee() -> void
{
}

#if 0

int main()
{
	std::cout << std::is_same<decltype(pf), decltype(pf1)>::value << std::endl;
	return 0;
}

#endif

