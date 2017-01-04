#include <iostream>
#include <algorithm>
#include <vector>
#include <numeric>
#include <array>

void test1() {
	std::vector<int> v = { 1, 3, 5, 7, 9 };
	auto isEven = [](int i) {return i % 2 == 0;};
	bool isallEven = std::all_of(v.begin(), v.end(), isEven);
	if (!isallEven)
		std::cout << "all is odd" << std::endl;

	bool isNoneEven = std::none_of(v.begin(), v.end(), isEven);
	if (isNoneEven)
		std::cout << "none is even" << std::endl;

	std::vector<int> v1 = { 1, 3, 5, 7, 8, 9 };
	bool anyof = std::any_of(v1.begin(), v1.end(), isEven);
	if (anyof)
		std::cout << "at least one is even" << std::endl;
}

void test2() {
	std::vector<int> v = { 1, 3, 5, 7, 9, 4 };
	auto isEven = [](int i) {return i % 2 == 0;};
	auto isOdd = [](int i) {return i % 2 != 0;};

	auto firstEven = std::find_if(v.begin(), v.end(), isEven);
	if (firstEven != v.end())
		std::cout << "the first even is " << *firstEven << std::endl;

	auto firstOdd = std::find_if(v.begin(), v.end(), isOdd);
	if (firstOdd != v.end())
		std::cout << "the first odd is " << *firstOdd << std::endl;

	auto odd = std::find_if_not(v.begin(), v.end(), isEven);
	if (odd != v.end())
		std::cout << "the first odd is " << *odd << std::endl;
}

void test3() {
	std::vector<int> v = { 1, 3, 5, 7, 9, 4 };
	std::vector<int> v1(v.size());
	//根据条件拷贝
	auto it = std::copy_if(v.begin(), v.end(), v1.begin(), [](int i) {return i%2!=0;});
	//缩减vector到合适大小
	v1.resize(std::distance(v1.begin(), it));
	for (int i : v1) {
		std::cout << i << " ";
	}

	std::cout << std::endl;
}

void test4() {
	std::vector<int> v(4);

	//直接通过iota初始化数组，更简洁
	std::iota(v.begin(), v.end(), 1);
	for (auto n : v) {
		std::cout << n << ' ';
	}
	std::cout << std::endl;

	std::array<int, 4> array;
	std::iota(array.begin(), array.end(), 1);
	for (auto n : array) {
		std::cout << n << ' ';
	}
	std::cout << std::endl;
}

void test5() {
	// your code goes here
	std::vector<int> v = { 1, 2, 5, 7, 9, 4 };
	auto result = std::minmax_element(v.begin(), v.end());

	std::cout << *result.first << " " << *result.second << std::endl;
}

void test6() {
	std::vector<int> v = { 1, 2, 5, 7, 9, 4 };
	auto pos = std::is_sorted_until(v.begin(), v.end());

	for (auto it = v.begin(); it != pos; ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;

	bool is_sort = std::is_sorted(v.begin(), v.end());
	std::cout << is_sort << std::endl;
}

//int main() {
//	test1();
//	test2();
//	test3();
//	test4();
//	test5();
//	test6();
//	return 0;
//}
