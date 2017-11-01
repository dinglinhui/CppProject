#if 1
#include <iostream>
namespace basic{

class A {
public:

    virtual ~A() {
    }

    virtual void f() {
        std::cout << "A::f " << m_iNum << std::endl;
    }

    int m_iNum;
};

class B: public A {
public:
    virtual ~B() {
    }

    virtual void f() {
        std::cout << "B::f " << m << std::endl;
    }

    int m;
};

class D: public A {
    virtual ~D() {
    }

};

void test_cast() {
    {
        A* pa = new A();
        B* pb = new B();
        pb->m_iNum = 100;
//		D* pd1=static_cast<D*>(pb);//compile error
        D* pd2 = dynamic_cast<D*>(pa); //pd2 is NULL

        delete pb;
        delete pa;
    }

    {
        A* pObjA = new A();
        B* pObjB = nullptr;
        pObjB = static_cast<B*>(pObjA);
        pObjB->m = 10;
        std::cout << pObjB << " " << pObjA << std::endl;
        delete pObjA;
    }

    class Base {
    public:
        virtual ~Base() {
        }
    };
    class Derived : public Base {
    public:
        virtual ~Derived() {
        }
    };

    {
        Base* b1 = new Derived;
        Base* b2 = new Base;
        Derived* d1 = dynamic_cast<Derived *>(b1);   // succeeds
        Derived* d2 = dynamic_cast<Derived *>(b2);  // fails: returns 'NULL'
        std::cout << d1 << " " << (d2 == nullptr ? "nullptr" : "not null") << std::endl;
    }
}
}

#endif
