#include "Target.h"
#include <iostream>
using namespace std;

namespace DesignPattern {
namespace AdapterPattern {

Target::Target() {
}

Target::~Target() {
}

void Target::Request() {
	cout << "Target::Request()" << endl;
}

} /* namespace AdapterPattern */
} /* namespace DesignPattern */
