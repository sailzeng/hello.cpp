// Hello.Cpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <iostream>
#include <intrin.h>

#include "main.h"

template< class O >
class C_O
{
};

template< class... T >
class multiobjs_pool
{
    //!

    //!
    std::tuple<C_O<T...> > pools_;
};

#include <bit>

//std::endian MY_ENDIAN = std::endian::native;
//std::endian LITTE_ENDIAN = std::endian::little;
//
//#if MY_ENDIAN == LITTE_ENDIAN
//uint32_t test_0001 = 1;
//#endif
//
//constexpr bool is_little_endian = (std::endian::native == std::endian::little);
//#if is_little_endian == true
//uint32_t test_0002 = 1;
//#endif

int main(int argc, char* argv[])
{
    std::cout << "C++ __cplusplus:" << __cplusplus << std::endl;
    std::cout << "--------------------------------" << std::endl;
    hello_regex_1(argc, argv);
    std::cout << "--------------------------------" << std::endl;

    /*
    hello_weak_ptr(argc,argv);
    test_atomic_rings(argc,argv);
    std::cout<<"--------------------------------"<<std::endl;
    test_variadic(argc, argv);
    std::cout << "--------------------------------" << std::endl;
    test_variadic2(argc,argv);
    std::cout << "--------------------------------" << std::endl;
    test_variadic3(argc,argv);
    std::cout << "--------------------------------" << std::endl;
    test_variadic4(argc,argv);
    std::cout << "--------------------------------" << std::endl;
    test_variadic5(argc,argv);
    */

    //test_thread_local(argc,argv);
    //hello_chrono_duration(argc, argv);
    //hello_chrono_timepoint(argc, argv);
    //hello_chrono_clock(argc, argv);
    //hello_progress_timer(argc, argv);
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

//size_t i = 0;
//size_t j = 0;
//for (i = 1; i < 102400; i++)
//{
//    j = i * 5 + 4;
//    if (j % 6 == 5 && j % 7 == 6)
//    {
//        break;
//    }
//}
//if (i != 102400)
//{
//    std::cout << j << std::endl;
//}
//else
//{
//    std::cout << "not " << std::endl;
//}
//
//using namespace std;
//
//unsigned long mask = 12;
//unsigned long index;
//unsigned char isNonzero;
//
//cout << "Enter a positive integer as the mask: " << flush;
//isNonzero = _BitScanReverse(&index, mask);
//if (isNonzero)
//{
//    cout << "Mask: " << mask << " Index: " << index << endl;
//}
//else
//{
//    cout << "No set bits found.  Mask is zero." << endl;
//}