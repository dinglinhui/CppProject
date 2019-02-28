/*
 * Lambda.cpp
 *
 *  Created on: Oct 9, 2014
 *      Author: xdinlin
 */
#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>

#if 0
struct S
{
    void f(int i);
};

void S::f(int i)
{
    //captures variables under enclosing scope
    [i]
    {

    };    // OK,captures variable i by value

    [=]
    {};    // OK,captures variables by value
    [&i]
    {};   // OK,captures variable i by reference
    [&]
    {};    // OK,captures variables by reference
    [this]
    {}; // OK,captures variable this pointer by value
    []
    {}; // OK,captures variables not at all

    [&, i]
    {};    // OK

    //    [&, &i]{};   // ERROR: i preceded by & when & is the default
    //    [=, this]{}; // ERROR: this when = is the default
    //    [i, i]{};    // ERROR: i repeated
}

//函数对象，或者仿函数
//简单的说就是重定义了成员函数operator()的一种自定义类型对象。
class Functor
{
public:
    Functor() :
    //			_i(0), _step(1)
    Functor(0, 1)
    {
    }

    Functor(int i, int step) :
    _i(i), _step(step)
    {
    }

    int operator()()
    {
        return (_i += _step);
    }

    int operator()(int x, int y, int z)
    {
        return (x + y + z);
    }
private:
    int _i;
    int _step;
};

void foo(std::function<int(int)> f)
{
    std::cout << "foo: " << f(5) << std::endl;
}

template<class Fn> void bar(Fn f)
{
    std::cout << "bar: " << f(5) << std::endl;
}

int main()
{
    /***************************************************************/
    //1
    auto f = [](int x, int y, int z)
    {   return x + y + z;};

    std::cout << f(1, 2, 3) << std::endl;

    //2
    Functor functor;
    std::cout << functor(1, 2, 3) << std::endl;

    //3
    auto fun1 = [](int x)
    {   return [](int y)
        {   return y * 2;}(x)+3;};

    std::function<int(int)> fun2 = [](int i)
    {   return i + 3;};

    std::function<int(int)> fun3 = [](int i)
    {   return i * 2;};

    foo(fun1);
    bar(fun1);
    std::cout << "fun2: " << fun2(fun3(5)) << '\n';

    /***************************************************************/
    std::vector<std::string> players
    {   "Rafael Nadal", "Novak Djokovic", "Roger Federer"};
    std::for_each(std::begin(players), std::end(players), [](std::string player)
            {
                //std::cout << player << std::endl;
            });

    /***************************************************************/
    //生成随机数字
    std::vector<int> numbers(10);
    std::generate(numbers.begin(), numbers.end(), []
            {
                return rand() % 100;
            });

    int odd_count = 0;
    std::for_each(numbers.begin(), numbers.end(), [&odd_count](int value)
            {
                if (value % 2 == 1) odd_count++;
            });

    //生成等差数列
    int step = 2;
    int i = -step;
    std::vector<int> seq(10);
    //
    std::generate(seq.begin(), seq.end(), [&i, step]() -> int
            {
                return (i += step);
            });

    //
    std::generate(seq.begin(), seq.end(), [i, step]() mutable -> int
            {
                return (i += step);
            });
    //
    std::generate(seq.begin(), seq.end(), Functor(i, step));

    /***************************************************************/
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
}

#endif
