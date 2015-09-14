/*
* SmartPtr.cpp
*
*  Created on: Oct 11, 2014
*      Author: xdinlin
*/

#include <iostream>
#include <memory>

#if 0
void check(std::weak_ptr<int> &wp)
{
	std::shared_ptr<int> sp = wp.lock(); // get data
	if (sp != nullptr)
		std::cout << "still " << *sp << std::endl;
	else
		std::cout << "pointer is invalid. " << std::endl;
}

int main(int argc, char** argv)
{
	/**********************************************************************/
	std::unique_ptr<int> up1(new int(11)); //uncopiable unique_ptr
	//	std::unique_ptr<int> up2 = up1; //can't pass compile
	std::cout << *up1 << std::endl;

	std::unique_ptr<int> up3 = std::move(up1); //now up3 get the data's unique
	std::cout << *up3 << std::endl;

	//	std::cout << *up1 << std::endl; // run error
	up3.reset(); //release
	up1.reset(); //no error
	//	std::cout << *up3 << std::endl;

	/**********************************************************************/
	std::shared_ptr<int> sp1(new int(22));
	std::shared_ptr<int> sp2 = sp1;
	std::cout << *sp1 << std::endl;
	std::cout << *sp2 << std::endl;

	sp1.reset();
	std::cout << *sp2 << std::endl;

	/**********************************************************************/
	std::weak_ptr<int> wp = sp2;
	check(wp);

	sp2.reset();
	check(wp);

	return 0;
}
#endif

