// Hello.Cpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "hello_cpp_main.h"

int main(int argc, char* argv[])
{
    std::cout<<"C++ __cplusplus:"<<__cplusplus<<std::endl;
    hello_align(argc,argv);
    
    std::cout<<"--------------------------------"<<std::endl;

    /*
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
