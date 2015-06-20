#ifndef __DYNAMIC_H__
#define __DYNAMIC_H__

#include <iostream>
#include <cstdio>
#include <cstring>
#include <typeinfo>

namespace OSUtils {
/* create object by class name */
void * Create(const char * class_name);

/* interface of class factory*/
class CAbstractFactory {
public:
	virtual ~CAbstractFactory() {
	}
	virtual void * Create(const char * class_name) = 0;
};

/* list of class factory */
class CFactoryList {
	friend void * Create(const char * class_name);

	static CFactoryList * _head;
	CFactoryList * m_next;
	CAbstractFactory * m_item;

public:
	/* ctor of CFactoryList, add a class factory to list */
	CFactoryList(CAbstractFactory * fact) :
			m_item(fact) {
		m_next = _head;
		_head = this;
	}
	virtual ~CFactoryList(void);
};

#if defined (_MSC_VER)
/* disable warning for the following line : CFactory( void ): m_item( this ) {} */
#pragma warning(disable : 4355)
#endif

/* realization of class factory */
template<typename Type>
class CFactory: public CAbstractFactory {
	Type _object;
	CFactoryList m_item;

public:
	/* add itself to list of class factory when constructed */
	CFactory(void) :
			m_item(this) {
	}

	virtual ~CFactory() {
	}

	/* create object of this class if matched */
	void * Create(const char * class_name) {
		std::string strClassName;

#if defined (WIN32)
		strClassName = ("class ");
#else
		char szSize[4] = {0};
		sprintf(szSize, "%d", (int)strlen(class_name));
		strClassName = szSize;
#endif
		strClassName += class_name;

		/* RTTI support */
		return !strcmp(typeid(_object).name(), strClassName.c_str()) ?
				(void *) (new Type) : nullptr;
	}
};
} // namespace OSUtils

namespace test {
void testDynclass();
}

#endif /* __DYNAMIC_H__ */
