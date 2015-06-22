/*
 * Fibonacci.hpp
 *
 *  Created on: Jun 19, 2015
 *      Author: dinglinhui
 */

#ifndef FIBONACCI_HPP_
#define FIBONACCI_HPP_

#include <cassert>

namespace utils {
int fibonacci1(int n) {     //递归实现
	assert(n > 0);

	if (n == 1 || n == 2)
		return 1;
	return fibonacci1(n - 1) + fibonacci1(n - 2);
}

int fibonacci2(int n) {   //数组实现
	assert(n > 0);

	if (n < 3)
		return 1;

	int *a = new int[n];
	a[0] = a[1] = 1;
	for (int i = 2; i < n; i++)
		a[i] = a[i - 1] + a[i - 2];
	int m = a[n - 1];
	delete a;         //释放内存空间
	return m;
}

int fibonacci3(int n) {          //借用vector<int>实现
	assert(n > 0);

	std::vector<int> a(2, 1);      //创建一个含有2个元素都为1的向量
	a.reserve(3);
	for (int i = 2; i < n; i++) {
		a.insert(a.begin(), a.at(0) + a.at(1));
		a.pop_back();
	}
	return a.at(0);
}

int fibonacci4(int n) {      //队列实现
	assert(n > 0);

	std::queue<int> q;
	q.push(1);
	q.push(1);
	for (int i = 2; i < n; i++) {
		q.push(q.front() + q.back());
		q.pop();
	}
	return q.back();
}

int fibonacci5(int n) {        //迭代实现
	assert(n > 0);

	int i, a = 1, b = 1, c = 1;
	for (i = 2; i < n; i++) {
		c = a + b;     //辗转相加法（类似于求最大公约数的辗转相除法）
		a = b;
		b = c;
	}
	return c;
}

double fibonacci6(int n) {
	auto gh5 = std::sqrt(5);
	return (pow((1 + gh5), n) - pow((1 - gh5), n)) / (pow((double) 2, n) * gh5);
}
}

#endif /* FIBONACCI_HPP_ */
