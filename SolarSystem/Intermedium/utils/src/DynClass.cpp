#include "DynClass.hpp"

namespace OSUtils {

CFactoryList * CFactoryList::_head = nullptr;

void *Create(const char * class_name) {
	void * new_object = 0;
	const CFactoryList * cur = CFactoryList::_head;

	for (; cur; cur = cur->m_next) {
		/* if class_name matched, object will then be created and returned */
		if ((new_object = cur->m_item->Create(class_name)) != nullptr) {
			break;
		}
	}

	return new_object;
}

/* delete linkage from CFactoryList when some class factory destroyed */
CFactoryList::~CFactoryList(void) {
	CFactoryList ** m_nextp = &CFactoryList::_head;

	for (; *m_nextp; m_nextp = &(*m_nextp)->m_next)
		if (*m_nextp == this) {
			*m_nextp = (*m_nextp)->m_next;
			break;
		}
}
} //namespace OSUtils

namespace test {

class CBase {
public:
	CBase() {
		std::cout << "CBase ctor" << std::endl;
	}
	virtual ~CBase() {
		std::cout << "CBase dtor" << std::endl;
	}
	void print() {
		PrintClassName();
	}
private:
	virtual void PrintClassName() {
		std::cout << "Base" << std::endl;
	}
};

class CDerived1: virtual public CBase {
public:
	CDerived1() {
		std::cout << "CDerived1 ctor" << std::endl;
	}
	virtual ~CDerived1() {
		std::cout << "CDerived1 dtor" << std::endl;
	}
private:
	virtual void PrintClassName() override {
		std::cout << "CDerived1" << std::endl;
	}
};
class CDerived2: virtual public CBase {
public:
	CDerived2() {
		std::cout << "CDerived2 ctor" << std::endl;
	}
	virtual ~CDerived2() {
		std::cout << "CDerived2 dtor" << std::endl;
	}
private:
	virtual void PrintClassName() override {
		std::cout << "CDerived2" << std::endl;
	}
};

class CDerived: public CDerived1, public CDerived2 {
public:
	CDerived() {
		std::cout << "CDerived ctor" << std::endl;
	}
	virtual ~CDerived() {
		std::cout << "CDerived dtor" << std::endl;
	}

private:
	void PrintClassName() override final {
		std::cout << "CDerived" << std::endl;
	}
};

void testDynclass() {
	OSUtils::CFactory<CDerived> factory1;
	CBase * p1 = (CBase *) OSUtils::Create("CDerived");
	if (p1 != nullptr)
		delete p1;
}
} // namespace test
