// Hello.Cpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "main.h"

#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>

template<int N> class C {};

constexpr int FivePlus(int x)
{
    return 5 + x;
}

void f(const int x)
{
    //C<x> c1; // Error: x is not compile-time evaluable.
    C<FivePlus(6)> c2; // OK
}

void g()
{
    const int x = 5;
    C<x> c1; // OK!!! 此处用x的「常量」语义
    *(int *)(&x) = 6; // Still OK! 只处用x的「只读」语义，去除const后可写
    C<x> c2; // Still OK! c2是C<5>类型（不是C<6>！）
    C<FivePlus(x)> c3; // Still OK! c3是C<10>类型（不是C<11>！）

    printf("%d\n", x); // 此处绝大多数（所有？）C++编译器会输出5！！
                       // （然而，如果用一个C编译器来编译类似代码，一定输出6）
    const int *p = &x;
    printf("%d\n", *p); // 此处，大多数C++编译器输出6
}