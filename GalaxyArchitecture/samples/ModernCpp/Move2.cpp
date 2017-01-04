/*
 * Move2.cpp
 *
 *  Created on: Oct 14, 2014
 *      Author: xdinlin
 */

#include <iostream>

class HasPtrMem {
public:
	HasPtrMem() :
			d(new int { 3 }) {
		std::cout << "constructor " << ++n_cstr << std::endl;
	}

	HasPtrMem(const HasPtrMem & copy) :
			d(new int { *copy.d }) {
		std::cout << "copy constructor " << ++n_cptr << std::endl;
	}

	HasPtrMem(HasPtrMem && move) :
			d(move.d) {
		move.d = nullptr;
		std::cout << "move constructor " << ++n_mvtr << std::endl;
	}

	HasPtrMem& operator=(HasPtrMem && move) {
		d = new int { *move.d };

		move.d = nullptr;
		std::cout << "move operator constructor " << ++n_mvtr << std::endl;
		return *this;
	}

	~HasPtrMem() {
		delete d;
		std::cout << "destructor " << ++n_dstr << std::endl;
	}

	int *d;
	static int n_cstr;
	static int n_dstr;
	static int n_cptr;
	static int n_mvtr;

};

int HasPtrMem::n_cstr { 0 };
int HasPtrMem::n_dstr { 0 };
int HasPtrMem::n_cptr { 0 };
int HasPtrMem::n_mvtr { 0 };

HasPtrMem getTemp() {
	HasPtrMem temp { };
	std::cout << "resource from " << ": " << std::hex << temp.d << std::endl;
	return temp;
}

#if 0
int main()
{
	HasPtrMem mem = getTemp();
	std::cout << "resource from " << ": " << std::hex << mem.d << std::endl;
	return 0;
}
#endif

