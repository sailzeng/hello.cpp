#include <variant>
#include <string>
#include <cassert>
#include <functional>
#include <iostream>

#if 1
struct TEST_A
{
    int a_;
    int b_;
    int c_;
};

//C++ 17
int test_variant(int argc, char* argv[])
{
    std::variant<int, float> v, w;
    v = 12; // v �� int
    int i = std::get<int>(v);
    w = std::get<int>(v);
    w = std::get<0>(v); // ��ǰһ��Ч����ͬ
    w = v; // ��ǰһ��Ч����ͬ

    //  std::get<double>(v); // ���� [int, float] ���� double
    //  std::get<3>(v);      // ���󣺺Ϸ��±�ֵΪ 0 �� 1

    try
    {
        float a = std::get<float>(w); // w �� int ���� float �����׳�
    }
    catch (const std::bad_variant_access&)
    {
    }

    using namespace std::literals;

    std::variant<std::string> x("abc"); // ת�����캯����������ʱ������
    x = "def"; // ת����ֵ��������ʱ��������

    std::variant<std::string, void const*> y("abc");
    // ���� char const * ʱת���� void const *
    // �ɹ�
    assert(std::holds_alternative<void const*>(y));
    y = "xyz"s;
    // �ɹ�
    assert(std::holds_alternative<std::string>(y));

    return 0;
}

int test_variant2(int argc, char* argv[])
{
    std::variant<int, double, std::string> v;
    v = 1;
    bool has_int = std::holds_alternative<int>(v);
    bool has_double = std::holds_alternative<double>(v);

    // 01011
    std::cout << "index : " << v.index() << " " << has_int << " " << has_double << " " << std::endl;
    std::cout << std::get<0>(v) << " " << *std::get_if<0>(&v) << std::endl;
    std::cout << std::endl;
    v = 2.0;
    std::cout << v.index() << " " << (std::get_if<int>(&v) == nullptr) << std::get<1>(v) << std::get<double>(v) << std::endl; // 1122
    std::cout << std::endl;
    v = "a";
    std::cout << v.index() << " " << std::get<2>(v) << std::get<std::string>(v) << std::endl; // 2aa
    std::cout << std::endl;
    return 0;
}

#endif