#ifndef MEMPOOL_HPP_
#define MEMPOOL_HPP_

#include <iostream>
#include <stdexcept>

namespace utils {
//内存池
template<typename T>
class Pool {
public:
	Pool(size_t element_num) {
		if (element_num <= 0) {
			throw std::out_of_range("index is out of range");
		}
		headoffreelist = (T *) new char(sizeof(T) * element_num);
		if (NULL == headoffreelist) {
			throw std::logic_error("new error");
		}
		int i = 0;
		for (; i < element_num - 1; i++) { //将内存串起来
			*(T**) (headoffreelist + i) = headoffreelist + i + 1;
		}
		*(T**) (headoffreelist + i) = NULL;
		pstart = headoffreelist;
	}

	~Pool(void) {
		delete (T *) pstart;
	}

	T *alloc(void) {
		if (nullptr == headoffreelist) {
			throw std::overflow_error("Out of memory");
		}
		T *p = headoffreelist;
		headoffreelist = *(T**) headoffreelist;
		return p;
	}

	void dealloc(T* p) {
		*(T **) p = headoffreelist;
		headoffreelist = p;
	}
private:
	void *pstart;       //记住分配的首地址，用于内存释放
	T *headoffreelist;  //未使用内存的首地址
};
}

namespace test {
using namespace utils;
//使用内存池的对象的大小不能小于一个指针的大小，否则会出错!
class Airplane {
public:
	Airplane(int number = 0) :
			number(number) {
	}
	static void* operator new(size_t size) {
		return mempool.alloc();
	}
	static void operator delete(void *p) {
		mempool.dealloc((Airplane*) p);
	}

private:
	static Pool<Airplane> mempool;
	int number;
};

Pool<Airplane> Airplane::mempool(10); //预先分配10个Airplane的内存空间

int testMempool() {
	try {
		Airplane *p1 = new Airplane();
		Airplane *p2 = new Airplane();
		Airplane *p3 = new Airplane();
		delete p3;
		Airplane *p4 = new Airplane();
		delete p1;
		delete p2;
		delete p4;

	} catch (std::exception const& ex) {
		std::cerr << "Exception: " << ex.what() << std::endl;
	}
	return 0;
}
}

#endif /* MEMPOOL_HPP_ */