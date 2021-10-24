
#include <memory>
#include <string>
#include <iostream>


//packed 和 align(alignas alignof) 是两个东西，


struct TEST_A1
{
    int a_;
    char b_;
    double c_;
};


struct alignas(32) TEST_A2
{
    int a;
    char b;
    double c_;
};

/*
ERROR :  warning C4359: 'TEST_A4': Alignment specifier is less than actual alignment (4), and will be ignored.
VS2019
struct alignas(1) TEST_A4
{
    int a;
    char b;
    double c_;
};
*/


#ifdef _WIN32

#pragma pack (1)
#pragma pack(show)
struct  TEST_A3
{
    int a_;
    char b_;
    double c_;
};
#pragma pack()
#pragma pack(show)


#else

struct  TEST_A3
{
    int a_;
    char b_;
    double c_;
} __attribute__((packed));

#endif



struct TEST_A5
{
    alignas(4) int a;
    alignas(1) char b;
    alignas(8) double c_;
};

//注意TEST_A6 和TEST_A2的区别。尺寸上的。
struct TEST_A6
{
    int a;
    char b;
    alignas(32) double c_;
};


struct TEST_A7
{
    int a;
    char b;
    alignas(128) double c_;
};


int hello_align(int argc,char* argv[])
{
    std::cout<<offsetof(TEST_A1,a_)<<std::endl;

    std::cout<<"Sizeof TEST_A1 "<<sizeof(TEST_A1)<<" Alignof(TEST_A1) "<<alignof(TEST_A1)<<std::endl;
    std::cout<<"Sizeof TEST_A2 "<<sizeof(TEST_A2)<<" Alignof(TEST_A2) "<<alignof(TEST_A2)<<std::endl;
    std::cout<<"Sizeof TEST_A3 "<<sizeof(TEST_A3)<<" Alignof(TEST_A3) "<<alignof(TEST_A3)<<std::endl;
    std::cout<<"Sizeof TEST_A5 "<<sizeof(TEST_A5)<<" Alignof(TEST_A5) "<<alignof(TEST_A5)<<std::endl;
    std::cout<<"Sizeof TEST_A6 "<<sizeof(TEST_A6)<<" Alignof(TEST_A6) "<<alignof(TEST_A6)<<std::endl;
    std::cout<<"Sizeof TEST_A7 "<<sizeof(TEST_A7)<<" Alignof(TEST_A7) "<<alignof(TEST_A7)<<std::endl;
    return 0;
}


