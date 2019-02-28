#include <iostream>
#include <vector>

#if 0
//1,对于函数来说，auto不能是其形参类型，无法通过编译
void foo(auto x = 1) {}

//2,对于结构体来说，非静态成员变量的类型不能是auto的。
struct str
{
	auto var = 10;
};

int main()
{
	//x为一个数组，y的类型是可以被推导的。
	char x[3];
	auto y = x;
	//3,声明auto数组同样会被编译器禁止。
	auto z[3] = x;

	//4,auto模版参数（实例化时），无法通过编译
	vector<auto> v = {1};
	return 0;
}
#endif
