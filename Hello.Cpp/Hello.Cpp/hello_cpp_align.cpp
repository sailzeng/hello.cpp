
#include <memory>
#include <string>
#include <iostream>


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

int hello_align(int argc,char* argv[])
{
    std::cout<<offsetof(TEST_A1,a)<<std::endl;

    std::cout<<"Sizeof TEST_A1 "<<sizeof(TEST_A1)<<" Alignof(TEST_A1) "<<alignof(TEST_A1)<<std::endl;
    std::cout<<"Sizeof TEST_A2 "<<sizeof(TEST_A2)<<" Alignof(TEST_A2) "<<alignof(TEST_A2)<<std::endl;
    std::cout<<"Sizeof TEST_A3 "<<sizeof(TEST_A3)<<" Alignof(TEST_A3) "<<alignof(TEST_A3)<<std::endl;
    return 0;
}


int test_a1(alignas(4) char *  a)
{
    return 0;
}