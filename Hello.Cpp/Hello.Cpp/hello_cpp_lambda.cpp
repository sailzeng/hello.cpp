// Hello.Cpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

int hello_lambda()
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

struct TEST_A1
{
	int a;
	char b;
};


struct alignas(32) TEST_A2
{
	int a;
	char b;
};

/*
ERROR :  warning C4359: 'TEST_A4': Alignment specifier is less than actual alignment (4), and will be ignored.
VS2019
struct alignas(1) TEST_A4
{
	int a;
	char b;
};

*/

#pragma pack (1)
#pragma pack(show)
struct  TEST_A3
{
	int a;
	char b;
};
#pragma pack()
#pragma pack(show)

int hello_align(int argc, char* argv[])
{
	std::cout << offsetof(TEST_A1, a) << std::endl;

	std::cout << "Sizeof TEST_A1 " << sizeof(TEST_A1) << " Alignof(TEST_A1) " << alignof(TEST_A1) << std::endl;
	std::cout << "Sizeof TEST_A2 " << sizeof(TEST_A2) << " Alignof(TEST_A2) " << alignof(TEST_A2) << std::endl;
	std::cout << "Sizeof TEST_A3 " << sizeof(TEST_A3) << " Alignof(TEST_A3) " << alignof(TEST_A3) << std::endl;
	return 0;
}
