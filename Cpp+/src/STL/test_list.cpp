#include <iostream>
#include <list>
using namespace std;

#if 0
int main() {
	list<int> a1 { 1, 2, 3, 4, 5 };
	a1.push_back(10);
	list<int>::iterator it;
	cout << "push_back:";
	for (it = a1.begin(); it != a1.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;

	a1.pop_back();
	cout << "pop_back:";
	for (it = a1.begin(); it != a1.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;

	a1.push_front(20);
	cout << "push_front:";
	for (it = a1.begin(); it != a1.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;

	a1.pop_front();
	cout << "pop_front:";
	for (it = a1.begin(); it != a1.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;

	return 0;
}
#endif
