#include <functional>
#include <queue>
#include <vector>
#include <iostream>

void process_value(int& i)
{
    std::cout << "LValue processed: " << i << std::endl;
}

void process_value(int&& i)
{
    std::cout << "RValue processed: " << i << std::endl;
}

void forward_value(int&& i)
{
    process_value(i);
}

int hello_rvalue_1(int argc, char* argv[])
{
    int a = 0;
    process_value(a);
    process_value(1);
    //
    forward_value(2);

    return 0;
}

class Intvec
{
public:
    explicit Intvec(size_t num = 0)
        : size_(num), data_(new int[size_])
    {
        log("constructor");
    }

    ~Intvec()
    {
        log("destructor");
        if (data_)
        {
            delete[] data_;
            data_ = 0;
        }
    }

    Intvec(const Intvec& other)
        : size_(other.size_), data_(new int[size_])
    {
        log("copy constructor");
        for (size_t i = 0; i < size_; ++i)
            data_[i] = other.data_[i];
    }

    Intvec& operator=(const Intvec& other)
    {
        log("copy assignment operator");
        Intvec tmp(other);
        std::swap(size_, tmp.size_);
        std::swap(data_, tmp.data_);
        return *this;
    }

    //注意noexcept
    Intvec(Intvec&& other) noexcept :
        size_(0),
        data_(nullptr)
    {
        log("move constructor");
        std::swap(size_, other.size_);
        std::swap(data_, other.data_);
        //是不是置位NULL 更好？下面也有这个问题
        //size_ = other.size_;
        //data_ = other.data_;
        //other.size_ = 0;
        //other.data_ = nullptr;
    }

    Intvec& operator=(Intvec&& other) noexcept
    {
        log("move assignment operator");
        std::swap(size_, other.size_);
        std::swap(data_, other.data_);
        return *this;
    }

private:

    void log(const char* msg)
    {
        std::cout << "[" << this << "] " << msg << "\n";
    }

    size_t size_;
    int* data_;
};

//universal references(通用引用)

template<typename T>
void f1(T&& param)
{
    if (std::is_same<std::string, T>::value)
        std::cout << "string" << std::endl;
    else if (std::is_same<std::string&, T>::value)
        std::cout << "string&" << std::endl;
    else if (std::is_same<std::string&&, T>::value)
        std::cout << "string&&" << std::endl;
    else if (std::is_same<int, T>::value)
        std::cout << "int" << std::endl;
    else if (std::is_same<int&, T>::value)
        std::cout << "int&" << std::endl;
    else if (std::is_same<int&&, T>::value)
        std::cout << "int&&" << std::endl;
    else
        std::cout << "unkown" << std::endl;
}

int hello_universal_references(int argc, char* argv[])
{
    int x = 1;
    f1(1); // 参数是右值 T推导成了int, 所以是int&& param, 右值引用
    f1(x); // 参数是左值 T推导成了int&, 所以是int&&& param, 折叠成 int&,左值引用
    int&& a = 2;
    f1(a); //虽然a是右值引用，但它还是一个左值， T推导成了int&
    std::string str = "hello";
    f1(str); //参数是左值 T推导成了string&
    f1(std::string("hello")); //参数是右值， T推导成了string
    f1(std::move(str));//参数是右值， T推导成了string

    return 0;
}

void RunCode(int&& m)
{
    std::cout << "rvalue ref" << std::endl;
}
void RunCode(int& m)
{
    std::cout << "lvalue ref" << std::endl;
}
void RunCode(const int&& m)
{
    std::cout << "const rvalue ref" << std::endl;
}
void RunCode(const int& m)
{
    std::cout << "const lvalue ref" << std::endl;
}

// 这里利用了universal references，如果写T&,就不支持传入右值，而写T&&，既能支持左值，又能支持右值
template<typename T>
void perfectForward(T&& t)
{
    //完美转发
    RunCode(std::forward<T>(t));
}

template<typename T>
void notPerfectForward(T&& t)
{
    RunCode(t);
}

int hello_perfect_forward(int argc, char* argv[])
{
    int a = 0;
    int b = 0;
    const int c = 0;
    const int d = 0;

    notPerfectForward(a); // lvalue ref
    notPerfectForward(std::move(b)); // lvalue ref
    notPerfectForward(c); // const lvalue ref
    notPerfectForward(std::move(d)); // const lvalue ref

    std::cout << "-------------------------------" << std::endl;
    perfectForward(a); // lvalue ref
    perfectForward(std::move(b)); // rvalue ref
    perfectForward(c); // const lvalue ref
    perfectForward(std::move(d)); // const rvalue ref

    return 0;
}

class RV_A
{
public:
    RV_A()
    {
        std::cout << "RV_A() " << std::endl;
    }
    ~RV_A()
    {
        std::cout << "~RV_A() " << std::endl;
    }
    RV_A(int a)
    {
        std::cout << "RV_A(int a) " << std::endl;
        a_ = a;
    }
    RV_A(const RV_A &o)
    {
        std::cout << "RV_A(const RV_A &o) " << std::endl;
        a_ = o.a_;
    }
    RV_A(RV_A &&o)
    {
        std::cout << "RV_A(RV_A &&o) " << std::endl;
        a_ = o.a_;
        o.a_ = 0;
    }

    RV_A& operator= (const RV_A& o)
    {
        std::cout << "RV_A& operator = (const RV_A& o) " << std::endl;
        a_ = o.a_;
        return *this;
    }
    RV_A& operator= (RV_A&& o)
    {
        std::cout << "RV_A& operator= (RV_A&& o) " << std::endl;
        a_ = o.a_;
        o.a_ = 0;
        return *this;
    }
public:
    //
    int a_ = 0;
};

template<typename T>
std::string type_to_string() {
#if defined(_MSC_VER)
    std::string type_name{ __FUNCSIG__ };
    // class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > __cdecl type_to_string<int&>(void)
    auto start_pos = type_name.find_first_of('<',
                                             std::string(typeid(std::string).name()).size()) + 1;
    auto end_pos = type_name.find_last_of('>');
    return type_name.substr(start_pos, (end_pos - start_pos));
#elif defined(__clang__)
    std::string type_name{ __PRETTY_FUNCTION__ };
    auto start_pos = type_name.find_first_of('=') + 2;
    auto end_pos = type_name.find_first_of(']', start_pos);
    return type_name.substr(start_pos, (end_pos - start_pos));

#elif defined(__GNUC__)
    std::string type_name{ __PRETTY_FUNCTION__ };
    // std::__cxx11::string type_to_string() [with T = int&; std::__cxx11::string = std::__cxx11::basic_string<char>]
    auto start_pos = type_name.find_first_of('=') + 2;
    auto end_pos = type_name.find_first_of(';', start_pos);
    return type_name.substr(start_pos, (end_pos - start_pos));
#endif
}

void test_rv_001(const RV_A &o)
{
    std::cout << __func__ << "------------------------------" << std::endl;
    RV_A a_1(o);
    RV_A a_2(std::move(o));
    std::cout << __func__ << "------------------------------" << std::endl;
}

void test_rv_002(RV_A &o)
{
    std::cout << __func__ << "------------------------------" << std::endl;
    RV_A a_1(o);
    RV_A a_2(std::move(o));
    std::cout << __func__ << "------------------------------" << std::endl;
}

void test_rv_003(RV_A &&o)
{
    std::cout << __func__ << "------------------------------" << std::endl;
    RV_A a_1(o);
    RV_A a_2(std::move(o));
    std::cout << __func__ << "------------------------------" << std::endl;
}

void test_rv_004(const RV_A &o)
{
    std::cout << __func__ << "------------------------------" << std::endl;
    RV_A a_1 = o;
    RV_A a_2(32);
    a_2 = o;
    RV_A a_3(64);
    a_3 = std::move(o);
    std::cout << __func__ << "------------------------------" << std::endl;
}

void test_rv_005(RV_A &o)
{
    std::cout << __func__ << "------------------------------" << std::endl;
    RV_A a_1 = o;
    RV_A a_2(32);
    a_2 = o;
    RV_A a_3(64);
    a_3 = std::move(o);
    std::cout << __func__ << "------------------------------" << std::endl;
}

void test_rv_006(RV_A &&o)
{
    std::cout << __func__ << "------------------------------" << std::endl;
    std::cout << "o type :" << type_to_string<decltype(o)>() << std::endl;
    std::cout << "o type :" << type_to_string<decltype(std::move(o))>() << std::endl;
    RV_A a_1 = o;
    RV_A a_2(32);
    a_2 = o;
    RV_A a_3(64);
    a_3 = std::move(o);
    std::cout << __func__ << "------------------------------" << std::endl;
}

void test_rv_007(RV_A &&o)
{
    std::cout << __func__ << "------------------------------" << std::endl;
    std::cout << "o type :" << type_to_string<decltype(o)>() << std::endl;
    RV_A a_1(o);
    std::cout << __func__ << "------------------------------" << std::endl;
}

int hello_right_value_001([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    RV_A a1(1024);
    test_rv_001(a1);
    RV_A a2(1024);
    test_rv_002(a2);
    RV_A a3(1024);
    test_rv_003(std::move(a3));
    RV_A a4(2048);
    test_rv_004(a4);
    RV_A a5(2048);
    test_rv_005(a5);
    RV_A a6(2048);
    test_rv_006(std::move(a6));
    RV_A a7(2048);
    RV_A a8(std::move(a7));
    test_rv_007(std::move(a7));
    return 0;
}