#pragma once
#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>

namespace basic {

template<typename Iterator>
void get_odds(Iterator begin, Iterator end) {
	std::for_each(begin, end, [](int n) {
		std::cout << n << std::endl;
	});

	auto pos = begin;
	while (pos != end) {
		pos = std::find_if(pos, end, [](int n) {
			return n % 2 == 1;
		});
		std::cout << *pos++ << " ";
	}

	std::cout << std::endl;
}

template<typename C>
void get_odds(C c) {
	get_odds(std::begin(c), std::end(c));
}

template<typename T, size_t N>
void get_odds(T (&arr)[N]) {
	get_odds(std::begin(arr), std::end(arr));
}

template<typename T1, typename T2>
auto add(T1 t1, T2 t2) -> decltype(t1 + t2) {
	static_assert(std::is_integral<T1>::value, "Type T1 must be integral");
	static_assert(std::is_integral<T2>::value, "Type T2 must be integral");
	return t1 + t2;
}

template<typename T>
void func(const T& t) {
	std::cout << t << std::endl;
}

template<typename T, typename ...Args>
void func(const T& t, Args ...args) {
	std::cout << t << std::endl;
	func(args...);
}

template<typename T>
T sum(T v) {
	return v;
}

template<typename T, typename ...Args>
T sum(T v, Args ...args) {
	return v + sum(args...);
}

///////////////////////////////////////////////////////////////////////////////
void printf(const char *s) {
	while (*s) {
		if (*s == '%' && *(++s) != '%')
			throw std::runtime_error(
					"invalid format string: missing arguments");
		std::cout << *s++;
	}
}

template<typename T, typename ... Args>
void printf(const char* s, T value, Args ... args) {
	while (*s) {
		if (*s == '%' && *(++s) != '%') {
			std::cout << value;
			printf(*s ? ++s : s, args...);
			return;
		}
		std::cout << *s++;
	}
	throw std::logic_error("extra arguments provided to printf");
}

template<typename T>
void print(T value) {
	std::cout << value << " ";
}

template<typename T, typename ... Args>
void print(T first, Args ... args) {
	std::cout << first << " ";
	print(args...);
}

template<typename ... Args>
void println(Args&&... args) {
	print(std::forward<Args>(args)...);
	std::cout << std::endl;
}

template<class T>
void print_list(std::initializer_list<T> il) {
	//	for (const T* it = begin(il); it != end(il); ++it)
	//		std::cout << ' ' << *it;
	std::for_each(std::begin(il), std::end(il), [](const T it) {
		std::cout << it << " ";
	});
	std::cout << '\n';
}

///////////////////////////////////////////////////////////////////////////////
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
auto apply_impl(F f, const T& t, std::index_sequence<I...>) -> decltype(f(std::get<I>(t)...)) {
	return f(std::get<I>(t)...);
}

template<typename F, typename T>
auto apply(F f, const T& t) -> decltype(apply_impl(f, t, std::make_index_sequence<std::tuple_size<T>::value>())) {
	return apply_impl(f, t, std::make_index_sequence<std::tuple_size<T>::value>());
}

///////////////////////////////////////////////////////////////////////////////
template<typename ... Arguments>
class VariadicTemplate {
public:
	void SampleFunction(Arguments ... params) {
	}
};

template<>
class VariadicTemplate<double, int, long> {
public:
	void SampleFunction(double param1, int param2, long param3) {
	}
};

///////////////////////////////////////////////////////////////////////////////
template<typename tStringType, typename tTraits = typename tStringType::traits_type>
void print_code_unit_sequence(tStringType str) {
	using char_type = typename tTraits::char_type;
	static_assert(std::is_same<char_type, char>::value ||
		std::is_same<char_type, char16_t>::value ||
		std::is_same<char_type, char32_t>::value, "error");
	using unsigned_char_type = typename std::make_unsigned<char_type>::type;
	using unsigned_int_type = typename std::make_unsigned<typename tTraits::int_type>::type;

	int w = std::is_same<char, char_type>::value ? 2 :
		std::is_same<char16_t, char_type>::value ? 4 : 8;
	for (auto c : str) {
		auto value = static_cast<unsigned_int_type>(static_cast<unsigned_char_type>(c));
		std::cout << "0x" << std::hex << std::uppercase << std::setw(w)
			<< std::setfill('0') << value << ' ';
	}
}

///////////////////////////////////////////////////////////////////////////////
void test_template() {
	int arr[] = { 1, 2, 3 };
	get_odds(arr);

	std::vector<int> v = { 1, 2, 3 };
	get_odds(v);

	func(1, 2, 3, 4, 5);
	std::cout << sum(1, 2, 3, 4, 5) << std::endl;

	//	#
	println(1);
	println(1, 2, 3, 4, 5);
	println("one", 2, "three", 4, "five");
	printf("one %d %s %f %s", 2, "three", 4, "five");
	print_list({ 100, 8, 15, 37, 26, 13, 27, 49, 55, 18 });

	/*//	#
	using namespace std::literals::string_literals;
	auto tuple1 = std::make_tuple(1, 'A', 1.2, "寒武纪的博客"s);
	auto result1 = apply(func1, tuple1);
	std::cout << "result1 = " << result1 << std::endl;

	auto tuple2 = std::make_tuple(1, 2);
	auto result2 = apply(func2, tuple2);
	std::cout << "result2 = " << result2 << std::endl;

	//	#
	std::string u8_str = u8"寒武纪";// utf-8
	std::u16string u16_str = u"寒武纪";// utf-16
	std::u32string u32_str = U"寒武纪";// ucs4
	std::cout << "utf-8: ";
	print_code_unit_sequence(u8_str);
	std::cout << std::endl;
	std::cout << "utf-16: ";
	print_code_unit_sequence(u16_str);
	std::cout << std::endl;
	std::cout << "ucs4: ";
	print_code_unit_sequence(u32_str);
	std::cout << std::endl;
	*/
}
} // namespace basic

