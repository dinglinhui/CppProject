#include "BasicClass.h"

namespace basic {

BasicClass::BasicClass() : 
    //ptr(new BasicCpp), // It's totally wrong here, it will cause endless loop 
    ref(*this) {
}

BasicClass::~BasicClass() {
}

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

void test_basic_cpp() {
    try {
        {
            Derived c;
            c.Operate_1();   // private inheritance
//          c.B::Operate();  // public inheritance
        }

        {
            int flag = 5;
            int i = 0;
            if (!flag && i++)
                std::cout << i << std::endl;
            ;
            std::cout << i << std::endl;
        }

        {
            unsigned int array[] = { 1, 1, 2, 2, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10 };
            std::cout << find(array, sizeof(array) / sizeof(unsigned int)) << std::endl;
        }

//  } catch (std::exception &e) {
//      std::cout << e.what() << std::endl;
    } catch (...) {
        std::cerr << "catch exception" << std::endl;
        throw;
    };
}

} /* namespace basic */
