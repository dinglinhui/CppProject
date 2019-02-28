#ifndef _BASIC_H_
#define _BASIC_H_
#include <iostream>
#include <cassert>

namespace basic {

class Memory {
public:
	Memory() :
			//ptr(new Memory), // It's totally wrong here, it will cause endless loop
			ref(*this) {
	}
	virtual ~Memory() {
	}

	void GetMemory(char **p, int num) {
		*p = new char[sizeof(char) * num];
	}

	void GetMemory(char *&p, int num) {
		p = new char[sizeof(char) * num];
	}

	char * GetMemory(int num) {
		char * p = new char[sizeof(char) * num];
		return p;
	}

private:
	Memory *ptr = nullptr;
	//Memory obj; //Basic didn't initialize completely until constructor finished .
	Memory &ref;
};

class Base;
class Another {
	friend class Base;
public:
	Another() {
	}

	~Another() {
	}

protected:
	void Operation() {
		std::cout << "Hello, I am here." << std::endl;
	}

};

class Base {
public:
	virtual ~Base() {
	}

	virtual void Operate() {
		a.Operation();
	}

	virtual void f() {
		std::cout << "Base::f " << m_iNum << std::endl;
	}

	int m_iNum;

private:
	Another a;
};

class DerivedA: private Base {
public:
	~DerivedA() {
	}

	void Operate_1() {
		Operate();
	}

	virtual void f() {
		std::cout << "DerivedA::f " << m << std::endl;
	}

	int m;
};

class DerivedB: public Base {
	virtual ~DerivedB() {
	}

};

unsigned int find(unsigned int *p, unsigned int num) {
	assert((p != nullptr) && (num != 0));
	unsigned int step = num / 2;
	step = (step % 2 == 0) ? step : step + 1;
	unsigned int * ptr = p + step;
	if (*ptr != *(ptr - 1)) {
		if (step == 2) {
			if (num == 3)
				return *ptr;
			return 0;
		} else {
			return find(ptr, num - step);
		}
	} else {
		if (step == 2) {
			return *(ptr - 2);
		} else {
			return find(p, step);
		}
	}
}

void test_basic() {
	try {
		{
			DerivedA c;
			c.Operate_1();   // private inheritance
//          c.B::Operate();  // public inheritance
		}

		{
			int flag = 5;
			int i = 0;
			if (!flag && i++)
				std::cout << i << std::endl;
			std::cout << i << std::endl;
		}

		{
			unsigned int array[] = { 1, 1, 2, 2, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10,
					10 };
			std::cout << find(array, sizeof(array) / sizeof(unsigned int))
					<< std::endl;
		}

//  } catch (std::exception &e) {
//      std::cout << e.what() << std::endl;
	} catch (...) {
		std::cerr << "catch exception" << std::endl;
		throw;
	};
}

void test_cast() {
	{
		Base* pa = new Base();
		DerivedB* pb = new DerivedB();
		pb->m_iNum = 100;
		//		DerivedB* pd1=static_cast<DerivedB*>(pb);//compile error
		DerivedB* pd2 = dynamic_cast<DerivedB*>(pa); //pd2 is NULL

		delete pb;
		delete pa;
	}

	{
		Base* pObjA = new Base();
		DerivedA* pObjB = nullptr;
		pObjB = static_cast<DerivedA*>(pObjA);
		pObjB->m = 10;
		std::cout << pObjB << " " << pObjA << std::endl;
		delete pObjA;
	}

	{
		Base* b1 = new DerivedA;
		Base* b2 = new Base;
		DerivedA* d1 = dynamic_cast<DerivedA *>(b1);   // succeeds
		DerivedA* d2 = dynamic_cast<DerivedA *>(b2); // fails: returns 'NULL'
		std::cout << d1 << " " << (d2 == nullptr ? "nullptr" : "not null")
				<< std::endl;
	}
}

} /* namespace basic */

#endif /* _BASIC_H_ */
