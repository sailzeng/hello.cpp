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
    C<x> c1; // OK!!! �˴���x�ġ�����������
    *(int *)(&x) = 6; // Still OK! ֻ����x�ġ�ֻ�������壬ȥ��const���д
    C<x> c2; // Still OK! c2��C<5>���ͣ�����C<6>����
    C<FivePlus(x)> c3; // Still OK! c3��C<10>���ͣ�����C<11>����

    printf("%d\n", x); // �˴�������������У���C++�����������5����
                       // ��Ȼ���������һ��C���������������ƴ��룬һ�����6��
    const int *p = &x;
    printf("%d\n", *p); // �˴��������C++���������6
}