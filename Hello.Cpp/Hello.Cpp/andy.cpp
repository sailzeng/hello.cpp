#include <any>
#include <vector>
#include <iostream>

//any��һ������������ݵ�ʵ�֡�
using std::any;

int test_any(int argc, char* argv[])
{
    try
    {
        // Integer 42:  Using the copy initialisation
        std::any value_ = 42;
        std::cout << "Value: "
            << std::any_cast<int>(value_) << std::endl;

        //bad_any_cast any�����ܽ����ڲ�ת��
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

        //any����ģ�¶�̬�������Եı�����ע�⣬any����ģ����
        std::vector<std::any>  va;
        va.push_back(std::any(9.0));
        va.push_back(std::any(11));
    }
    //ת��ʧ�ܻ��׳��쳣
    catch (std::bad_any_cast& e)
    {
        std::cout << "\n"
            << e.what() << std::endl;
    }
    return 0;
}