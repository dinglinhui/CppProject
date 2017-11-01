#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>

namespace basic {

template<typename Iterator>
void bar(Iterator begin, Iterator end) {
    std::for_each(begin, end, [](int n) {
        std::cout << n << std::endl;
    });

    auto is_odd = [](int n) {
        return n%2==1;
    };
    auto pos = std::find_if(begin, end, is_odd);
    if (pos != end)
        std::cout << *pos << std::endl;
}

template<typename C>
void foo(C c) {
    bar(std::begin(c), std::end(c));
}

template<typename T, size_t N>
void foo(T (&arr)[N]) {
    bar(std::begin(arr), std::end(arr));
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

void printf(const char *s) {
    while (*s) {
        if (*s == '%' && *(++s) != '%')
            throw std::runtime_error("invalid format string: missing arguments");
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

int c_function(int a, int b) {
    return a + b;
}

class Functor {
public:
    int operator()(int a, int b) {
        return a + b;
    }
};

void test_template() {
    {
        int arr[] = { 1, 2, 3 };
        foo(arr);

        std::vector<int> v;
        v.push_back(1);
        v.push_back(2);
        v.push_back(3);
        foo(v);
    }
    std::cout << std::endl;

    {
        int (*f)(int, int) = c_function;
        std::cout << f(3, 4) << std::endl;

        Functor ff = Functor();
        std::cout << ff(3, 4) << std::endl;

        std::function<int(int)> fib = [&fib](int n)
        {   return n < 2 ? 1 : fib(n-1) + fib(n-2);};
        std::cout << fib(12) << std::endl;

        // an array of functions:
        std::function<int(int, int)> fn[] = { std::plus<int>(), std::minus<int>(), std::divides<int>(), std::multiplies<int>() };
        for (auto& x : fn)
            std::cout << x(10, 5) << '\n';

        func(1, 2, 3, 4, 5);
        std::cout << sum(1, 2, 3, 4, 5) << std::endl;
    }
}
}

