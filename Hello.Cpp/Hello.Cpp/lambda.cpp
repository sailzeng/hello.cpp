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


void f1(int (*)(int)) {}
void f2(char (*)(int)) {}
void h(int (*)(int)) {} // #1
void h(char (*)(int)) {} // #2

int hello_lambda_2(int argc,char* argv[])
{
    // 泛型 lambda，operator() 是有两个形参的模板
    auto glambda=[](auto a,auto&& b) { return a<b; };
    bool b=glambda(3,3.14); // ok


    
    auto glambda3=[](auto a) { return a; };

    f1(glambda3); // ok
    //f2(glambda3); // error: not convertible
    h(glambda3); // ok: calls #1 since #2 is not convertible

    int& (*fpi)(int*)=[](auto* a)->auto & { return *a; }; // ok

    // generic lambda, operator() is a template with two parameters
    //C++ 20
    //auto glambda2=[]<class T>(T a,auto&&b) { return a<b; };

    // 输出：20 100
    return 0;

}

