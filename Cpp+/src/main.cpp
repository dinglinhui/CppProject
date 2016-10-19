/*
 * main.cpp
 *
 *  Created on: Jun 4, 2015
 *      Author: dlh
 */

#include "Data Structure/ObjectManager.h"
#include "Data Structure/StaticMemory.h"
#include "Data Structure/Events.hpp"
#include "Data Structure/Range.hpp"
#include "Data Structure/Optional.hpp"
#include "Data Structure/CommCommand.hpp"
#include "Data Structure/ObjectPool.hpp"
#include "Modern C++/Timer.hpp"
#include "Modern C++/ThreadPool.hpp"

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <random>
#include <chrono>
#include <initializer_list>   // std::initializer_list
#include <tuple>
#include <iostream>
#include <string>
#include <utility>
#include <iomanip>
#include <regex>
#include <type_traits>

typedef struct tag_Student {
	int sId;
	int sex;
	std::string name;
	std::string classname;
} Student;

template<typename tStringType,
		typename tTraits = typename tStringType::traits_type>
void print_code_unit_sequence(tStringType str) {
	using char_type = typename tTraits::char_type;
	static_assert(std::is_same<char_type, char>::value || std::is_same<char_type, char16_t>::value || std::is_same<char_type, char32_t>::value, "error");
	using unsigned_char_type = typename std::make_unsigned<char_type>::type;
	using unsigned_int_type = typename std::make_unsigned<typename tTraits::int_type>::type;
	int w = std::is_same<char, char_type>::value ? 2 :
			std::is_same<char16_t, char_type>::value ? 4 : 8;
	for (auto c : str) {
		auto value =
				static_cast<unsigned_int_type>(static_cast<unsigned_char_type>(c));
		std::cout << "0x" << std::hex << std::uppercase << std::setw(w)
				<< std::setfill('0') << value << ' ';
	}
}

int func1(int arg1, char arg2, double arg3, const std::string& arg4) {
	std::cout << "call func1(" << arg1 << ", " << arg2 << ", " << arg3 << ", "
			<< arg4 << ")" << std::endl;
	return 0;
}

int func2(int arg1, int arg2) {
	std::cout << "call func2(" << arg1 << ", " << arg2 << ")" << std::endl;
	return arg1 + arg2;
}

template<typename F, typename T, std::size_t ... I>
auto apply_impl(F f, const T& t,
		std::index_sequence<I...>) -> decltype(f(std::get<I>(t)...)) {
	return f(std::get<I>(t)...);
}

template<typename F, typename T>
auto apply(F f,
		const T& t) -> decltype(apply_impl(f, t, std::make_index_sequence<std::tuple_size<T>::value>())) {
	return apply_impl(f, t,
			std::make_index_sequence<std::tuple_size<T>::value>());
}

template<class T> void print_list(std::initializer_list<T> il) {
//	for (const T* it = begin(il); it != end(il); ++it)
//		std::cout << ' ' << *it;
	std::for_each(std::begin(il), std::end(il), [](const T it) {
		std::cout << it << " ";
	});
	std::cout << '\n';
}

void swap(int &p, int & q) {
	int temp = p;
	p = q;
	q = temp;
}

int partition(int ArrayInput[], int nLow, int nHigh) {
	int nTemp = ArrayInput[nHigh];
	int i = nLow, j = nLow - 1;
	for (; i < nHigh; i++) {
		if (ArrayInput[i] <= nTemp) {
			j++;
			if (i != j) {
				swap(ArrayInput[i], ArrayInput[j]);
			}
		}
	}
	swap(ArrayInput[j + 1], ArrayInput[nHigh]);
	return (j + 1);
}

void quick_sort(int ArrayInput[], int nLow, int nHigh) {
	if (nLow < nHigh) {
		int nIndex = partition(ArrayInput, nLow, nHigh);
		quick_sort(ArrayInput, nLow, nIndex - 1);
		quick_sort(ArrayInput, nIndex + 1, nHigh);
	}
}

void merge_array(int a[], int first, int mid, int last, int c[]) {
	int i = first, j = mid + 1, k = 0;
	while (i <= mid && j <= last) {
		if (a[i] > a[j])
			c[k++] = a[j++];
		else
			c[k++] = a[i++];
	}
	while (i <= mid)
		c[k++] = a[i++];
	while (j <= last)
		c[k++] = a[j++];
	for (int x = 0; x < k; x++)
		a[first + x] = c[x];
}

void merge_sort(int a[], int first, int last, int temp[]) {
	if (first < last) {
		int mid = (first + last) / 2;
		merge_sort(a, first, mid, temp);
		merge_sort(a, mid + 1, last, temp);
		merge_array(a, first, mid, last, temp);
	}
}

#if 0

int main() {

	try {
//		float coords[cplusplus::global::three_dimension] = { 1.2, 3.4, 5.6 };
//		cplusplus::test::testPoint(coords);
//		cplusplus::test::testEvents();
//		cplusplus::test::testTimer();
//		cplusplus::test::testOptional();
//		cplusplus::test::testWrap();
//		cplusplus::test::testObjectPool();
		cplusplus::test::testThreadPool();

	} catch (std::exception const& ex) {
		std::cerr << "Exception: " << ex.what() << std::endl;
		return -1;
	}

	//	print_list( { 100, 8, 15, 37, 26, 13, 27, 49, 55, 18 });
	//	#1 C Style
	int array[] = { 100, 8, 15, 37, 26, 13, 27, 49, 55, 18 };
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

	//	# 获取当前路径
	char *buffer;
	if ((buffer = getcwd(NULL, 0)) == NULL) {
		perror("getcwd error");
	} else {
		std::cout << buffer << std::endl;
		free(buffer);
	}

	//	#
	using namespace std::literals::string_literals;
	auto tuple1 = std::make_tuple(1, 'A', 1.2, "破晓的博客"s);
	auto result1 = apply(func1, tuple1);
	std::cout << "result1 = " << result1 << std::endl;

	auto tuple2 = std::make_tuple(1, 2);
	auto result2 = apply(func2, tuple2);
	std::cout << "result2 = " << result2 << std::endl;

	//	#
	std::string u8_str = u8"𪚥"; // utf-8
	std::u16string u16_str = u"𪚥"; // utf-16
	std::u32string u32_str = U"𪚥"; // ucs4
	std::cout << "utf-8: ";
	print_code_unit_sequence(u8_str);
	std::cout << std::endl;
	std::cout << "utf-16: ";
	print_code_unit_sequence(u16_str);
	std::cout << std::endl;
	std::cout << "ucs4: ";
	print_code_unit_sequence(u32_str);
	std::cout << std::endl;

	//	＃
	//data为已经从srt文件中读取的文本
	std::string data =
			"1\r\n00:00:53,477 --> 00:00:54,845\r\n大唐\r\n\r\n2\r\n00:00:55,212 --> 00:00:57,280\r\n我夙夜忧思的母国\r\n\r\n3\r\n00:00:58,615 --> 00:00:59,750\r\n洛阳\r\n\r\n4\r\n00:01:00,250 --> 00:01:02,419\r\n我魂萦梦系之地\r\n\r\n5\r\n00:01:05,389 --> 00:01:06,890\r\n三十岁那年\r\n\r\n";
	//如果编译器不支持C++11的raw string literals 下面一句可以写成 std::regex regex("(\\d+)\\r\\n(\\d+:\\d{2}:\\d{2},\\d{3})\\s-->\\s(\\d+:\\d{2}:\\d{2},\\d{3})\\r\\n(.*)\\r\\n\\r\\n");
	std::regex regex(
			R"((\d+)\r\n(\d+:\d{2}:\d{2},\d{3})\s-->\s(\d+:\d{2}:\d{2},\d{3})\r\n(.*)\r\n\r\n)");
	std::match_results<std::string::const_iterator> match_results;
	auto begin = data.cbegin();
	std::vector<std::tuple<std::string, std::string, std::string, std::string>> vec;
	while (begin != data.cend()) {
		if (std::regex_search(begin, data.cend(), match_results, regex)) {
			vec.emplace_back(match_results[1], match_results[2],
					match_results[3], match_results[4]);
			begin = match_results.suffix().first;
		} else {
			break;
		}
	}
	for (const auto& t : vec) {
		std::cout << "序号: " << std::get<0>(t) << " 开始时间: " << std::get<1>(t)
				<< " 结束时间: " << std::get<2>(t) << " 文本: " << std::get<3>(t)
				<< std::endl;
	}

	std::cout << "object address:"
			<< ObjectManager<Student>::Create(new StaticMemory()) << " size:"
			<< sizeof(Student) << std::endl;

	return 0;
}
#endif

