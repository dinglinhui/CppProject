#if 1

#include <iostream>
using namespace std;

namespace basic {

class Test {
public:
    Test(int x) :
            x_(x), outputTimes_(0) {
    }

    int Getx() const {
        //x_ = 100 ;// Error
        cout << "const Getx..." << endl;
        return x_;
    }

    int Getx() {
        cout << "Getx..." << endl;
        return x_;
    }

    int Setx() {
        return x_;
    }

    void Output() const {
        //x_ = 102; //Error
        cout << "x = " << x_ << endl;
        outputTimes_++;
    }

    int GetOutputTimes() const {
        return outputTimes_;
    }

private:
    int x_;
    mutable int outputTimes_;
};

void test_const() {
    const Test t(10);
    t.Getx();

    //t.Setx(); //Error

    Test t2(20);
    t2.Getx();
    t2.Output();
    t2.Output();

    cout << "outputTimes= " << t2.GetOutputTimes() << endl;
}
} /* namespace basic */

#endif
