#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>

#if 0
int main()
{
    std::vector<int> c
    {   1, 2, 3, 4, 5, 6, 7};

    int x = 5;
    c.erase(std::remove_if(c.begin(), c.end(), [x](int n)
                    {   return n < x;}), c.end());

    std::cout << "c: ";
    for (auto i : c)
    {
        std::cout << i << ' ';
    }
    std::cout << '\n';

    // the type of a closure cannot be named, but can be inferred with auto
    auto func1 = [](int i)
    {   return i+4;};
    std::cout << "func1: " << func1(6) << '\n';

    // like all callable objects, closures can be captured in std::function
    // (this may incur unnecessary overhead)
    std::function<int(int)> func2 = [](int i)
    {   return i+4;};
    std::cout << "func2: " << func2(6) << '\n';


	//	#1 C Style
	int array[] = {100, 8, 15, 37, 26, 13, 27, 49, 55, 18};
	for (int i = 0; i < sizeof(array) / sizeof(int); i++)
	std::cout << array[i] << " ";
	std::cout << std::endl;

	//	#2 C++ 11 for
	quick_sort(array, 0, sizeof(array) / sizeof(int) - 1);
	for (const int &x : array)
	std::cout << x << " ";
	std::cout << std::endl;

	//	#3 C++ 11 shuffle and C++ 98/03 iterator
	std::vector<int> vector_array(array, array + sizeof(array) / sizeof(int));
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(std::begin(vector_array), std::end(vector_array),
			std::default_random_engine(seed));
	for (std::vector<int>::iterator it = vector_array.begin();
			it != vector_array.end(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;

	//	#4 C++ 11 Style
	std::sort(std::begin(vector_array), std::end(vector_array),
			[](const int a, const int b) {
				return a < b;
			});

	std::for_each(std::begin(vector_array), std::end(vector_array),
			[](const int element) {
				std::cout << element << " ";
			});
	std::cout << std::endl;

}

#endif
