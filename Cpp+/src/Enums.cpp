/*
 * Enums.cpp
 *
 *  Created on: Oct 15, 2014
 *      Author: xdinlin
 */

#include <iostream>

//1,宏的弱点在于其定义的只是预处理阶段的名字，如果代码中有Male或者Female的字符串，无论在什么位置一律将被替换。
//这在有的时候会干扰到正常的代码，因此很多时候为了避免这种情况，程序员会让宏全部以大写来命名，以区别于正常的代码。
/*
 #define Male 0
 #define Female 1
 */

//2,这里匿名枚举中的Male和Female都是编译时期的名字，会得到编译器的检查。相比于宏的实现，匿名枚举不会有干扰正常代码的尴尬。
/*enum
 {
 Male, Female
 };*/

//3,同样得到编译时期的检查，由于是静态变量，其名字作用域也被很好的局限在文件内。不过相比于enum，静态常量不仅仅是一个编译时期的名字，
//编译器还可能会为变量在目标代码中产生实际的数据，相比而言，匿名的枚举视乎更为好用。
/*const static int Male = 0;
 const static int Female = 1;*/

/*
 enum Type
 {
 General, Light, Medium, Heavy
 };

 enum Category
 {
 General = 1, Pistol, MechineGun, Cannon
 };
 */

enum class Type {
	General, Light, Medium, Heavy
};

enum class Category {
	General = 1, Pistol, MechineGun, Cannon
};

#if 0
int main()
{
	Type t = Type::Light;
	//	t = General;

	if (t == Category::General)
	std::cout << "general weapon" << std::endl;

	if (t > Type::General)
	std::cout << "not general weapon" << std::endl;

	if (t > 0)
	std::cout << "not general weapon" << std::endl;

	if ((int)t > 0)
	std::cout << "not general weapon" << std::endl;

	return 0;
}

#endif
