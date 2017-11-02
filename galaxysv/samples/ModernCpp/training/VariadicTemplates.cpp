#include <iostream>
#include <tuple>
#include <functional>
#include <string>

template<class ... T>
void f(T ... args) {
	std::cout << sizeof...(args) << std::endl;
}
///////////////////////////////////////////////////////////////////////////////
void print() {
	std::cout << "empty" << std::endl;
}
template<class T, class ...Args>
void print(T head, Args ... rest) {
	std::cout << "parameter " << head << std::endl;
	print(rest...);
}
///////////////////////////////////////////////////////////////////////////////
template<typename T>
T sum(T t) {
	return t;
}
template<typename T, typename ... Types>
T sum(T first, Types ... rest) {
	return first + sum<T>(rest...);
}
///////////////////////////////////////////////////////////////////////////////
template<class T>
void printarg(T t) {
	std::cout << t << std::endl;
}
template<class ...Args>
void expand(Args ... args) {
	int arr[] = { (printarg(args), 0)... };
}
///////////////////////////////////////////////////////////////////////////////
template<class F, class ... Args>
void expand(const F& f, Args&&...args) {
	std::initializer_list<int> { (f(std::forward< Args>(args)),0)... }; //完美转发
}

///////////////////////////////////////////////////////////////////////////////
//前向声明
template<typename ... Args>
struct Sum;

//基本定义
template<typename First, typename ... Rest>
struct Sum<First, Rest...> {
	enum {
		value = Sum<First>::value + Sum<Rest...>::value
	};
};

//递归终止1
template<>
struct Sum<> {
	enum {
		value = 0
	};
};
//递归终止2
template<typename Last>
struct Sum<Last> {
	enum {
		value = sizeof(Last)
	};
};
//递归终止3
template<typename First, typename Last>
struct Sum<First, Last> {
	enum {
		value = sizeof(First) + sizeof(Last)
	};
};

///////////////////////////////////////////////////////////////////////////////
//前向声明
//template<typename First, typename ... Args>
//struct Sum1;
//
////基本定义
//template<typename First, typename ... Rest>
//struct Sum1<First, Rest...> : std::integral_constant<int,
//		Sum1<First>::value + Sum1<Rest...>::value> {
//};
//
////递归终止
//template<typename Last>
//struct Sum1<Last> : std::integral_constant<int, sizeof(Last)> {
//};

///////////////////////////////////////////////////////////////////////////////
//整型序列的定义
template<int...>
struct IndexSeq {};

//继承方式，开始展开参数包
template<int N, int ... Indexes>
struct MakeIndexes: MakeIndexes<N - 1, N - 1, Indexes...> {
};

//模板特化，终止展开参数包的条件
template<int ... Indexes>
struct MakeIndexes<0, Indexes...> {
	typedef IndexSeq<Indexes...> type;
};
///////////////////////////////////////////////////////////////////////////////
template<typename T, typename ... Args>
T* Instance(Args&&... args) {
	return new T(std::forward<Args>(args)...);
}

struct A {
	A(int) {
	}
};

struct B {
	B(int, double) {
	}
};
///////////////////////////////////////////////////////////////////////////////
struct App {
	void Fun(int i) {
		std::cout << i << std::endl;
	}
	void Fun1(int i, double j) {
		std::cout << i + j << std::endl;
	}
};

template<class T, class R, typename ... Args>
class MyDelegate {
public:
	MyDelegate(T* t, R (T::*f)(Args...)) :
			m_t(t), m_f(f) {
	}

	R operator()(Args&&... args) {
		return (m_t->*m_f)(std::forward<Args>(args) ...);
	}

private:
	T* m_t;
	R (T::*m_f)(Args...);
};

template<class T, class R, typename ... Args>
MyDelegate<T, R, Args...> CreateDelegate(T* t, R (T::*f)(Args...)) {
	return MyDelegate<T, R, Args...>(t, f);
}
///////////////////////////////////////////////////////////////////////////////
#if 0
int main() {
//	f();
//	f(1, 2);
//	f(1, 2.5, "");
//	print(1, 2, 3, 4);
//	expand(1, 2, 3, 4);
//	expand([](int i) {std::cout<<i<<std::endl;}, 1, 2, 3);

//	std::tuple<> tp;
//	std::tuple<int> tp1 = std::make_tuple(1);
//	std::tuple<int, double> tp2 = std::make_tuple(1, 2.5);
//	std::tuple<int, double, std::string> tp3 = std::make_tuple(1, 2.5, "");

//	Sum1<int, double, short>::value; //值为14
//	using T = MakeIndexes<3>::type;
//	std::cout << typeid(T).name() << std::endl;
//	A* pa = Instance<A>(1);
//	B* pb = Instance<B>(1, 2);
	App a;
	auto d = CreateDelegate(&a, &App::Fun); //创建委托
	d(1); //调用委托，将输出1
	auto d1 = CreateDelegate(&a, &App::Fun1); //创建委托
	d1(1, 2.5); //调用委托，将输出3.5
	return 0;
}

#endif

