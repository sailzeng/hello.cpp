#include <any>
#include <vector>
#include <iostream>

//any是一个存放任意数据的实现。
using std::any;

int test_any(int argc, char* argv[])
{
    try
    {
        // Integer 42:  Using the copy initialisation
        std::any value_ = 42;
        std::cout << "Value: "
            << std::any_cast<int>(value_) << std::endl;

        //bad_any_cast any并不能进行内部转换
        //std::cout << "Value: "
        //	<< std::any_cast<double>(value) << std::endl;

        // Using the assignment operator
        // to store a string
        value_ = "Hello World";
        std::cout << "Value: "
            << std::any_cast<const char*>(value_) << std::endl;

        // Using the parametrized constructor
        any val(19.0);
        std::cout << "Value: "
            << std::any_cast<double>(val) << std::endl;
        //bad_any_cast
        //std::cout << "Value: "
        //	<< std::any_cast<int>(val) << std::endl;

        any val_brace{ std::string("Brace Initialisation") };
        std::cout << "Value: "
            << std::any_cast<std::string>(val_brace) << std::endl;

        //any可以模仿动态类型语言的变量。注意，any不是模板类
        std::vector<std::any>  va;
        va.push_back(std::any(9.0));
        va.push_back(std::any(11));
    }
    //转换失败会抛出异常
    catch (std::bad_any_cast& e)
    {
        std::cout << "\n"
            << e.what() << std::endl;
    }
    return 0;
}

struct A_S_1
{
    int a1_1_ = 0;
    int a1_2_ = 0;
};

struct A_S_2
{
    int a2_1_ = 0;
    int a2_2_ = 0;
};

struct A_S_3
{
    int a3_1_ = 0;
    int a3_2_ = 0;
};

int anydata_to_value(std::any & a_data)
{
    if (a_data.type() == typeid(A_S_1))
    {
        A_S_1 a_s_1 = std::any_cast<A_S_1>(a_data);
        std::cout << "A_S_1" << std::endl;
    }
    else if (a_data.type() == typeid(A_S_2))
    {
        A_S_2 a_s_2 = std::any_cast<A_S_2>(a_data);
        std::cout << "A_S_2" << std::endl;
    }
    else if (a_data.type() == typeid(A_S_3))
    {
        A_S_3 a_s_3 = std::any_cast<A_S_3>(a_data);
        std::cout << "A_S_3" << std::endl;
    }
    else
    {
    }
    return 0;
}

int test_any2(int argc, char* argv[])
{
    std::vector<std::any> any_ary;
    A_S_1 a_s_1;
    any_ary.push_back(std::any(a_s_1));
    A_S_2 a_s_2;
    any_ary.push_back(std::any(a_s_2));
    A_S_3 a_s_3;
    any_ary.push_back(std::any(a_s_3));

    for (auto &ref_ary : any_ary)
    {
        anydata_to_value(ref_ary);
    }

    return 0;
}