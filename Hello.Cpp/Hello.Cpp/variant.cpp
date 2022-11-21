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
    v = 12; // v 含 int
    int i = std::get<int>(v);
    w = std::get<int>(v);
    w = std::get<0>(v); // 与前一行效果相同
    w = v; // 与前一行效果相同

    //  std::get<double>(v); // 错误： [int, float] 中无 double
    //  std::get<3>(v);      // 错误：合法下标值为 0 与 1

    try
    {
        float a = std::get<float>(w); // w 含 int 而非 float ：将抛出
    }
    catch (const std::bad_variant_access&)
    {
    }

    using namespace std::literals;

    std::variant<std::string> x("abc"); // 转换构造函数在无歧义时起作用
    x = "def"; // 转换赋值在无歧义时亦起作用

    std::variant<std::string, void const*> y("abc");
    // 传递 char const * 时转换成 void const *
    // 成功
    assert(std::holds_alternative<void const*>(y));
    y = "xyz"s;
    // 成功
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