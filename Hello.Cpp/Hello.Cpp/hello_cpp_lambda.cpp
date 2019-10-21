// Hello.Cpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

int hello_lambda(int argc, char* argv[])
{
    std::cout << "Hello World!\n"; 

	int x = 10;

	// 复制捕捉x
	auto add_x = [x](int a) { return a + x; };  

	std::cout << x << std::endl;
	// 引用捕捉x
	auto multiply_x = [&x](int a) { x = 30; return a * x; };

	std::cout << add_x(10) << std::endl;
	std::cout << x << std::endl;

	x = 20;
	std::cout << add_x(10) << std::endl;
	std::cout << x << std::endl;

	std::cout << multiply_x(10) << std::endl;
	std::cout << x << std::endl;

	// 输出：20 100
	return 0;

}

