#include <random>
#include <iostream>
#include <memory>
#include <functional>
#include <unordered_map>
#include <unordered_set>

void f(int n1, int n2, int n3, const int& n4, int n5)
{
    std::cout << n1 << ' ' << n2 << ' ' << n3 << ' ' << n4 << ' ' << n5 << '\n';
}

int g(int n1)
{
    return n1;
}

struct Foo
{
    void print_sum(int n1, int n2)
    {
        std::cout << n1 + n2 << '\n';
    }
    int data = 10;
};

int test_bind(int argc, char* argv[])
{
    // for _1, _2, _3...
    using namespace std::placeholders;

    // demonstrates argument reordering and pass-by-reference
    int n = 7;
    // (_1 and _2 are from std::placeholders, and represent future
    // arguments that will be passed to f1)
    auto f1 = std::bind(f, _2, _1, 42, std::cref(n), n);
    n = 10;
    f1(1, 2, 1001); // 1 is bound by _1, 2 is bound by _2, 1001 is unused
    // makes a call to f(2, 1, 42, n, 7)

// nested bind subexpressions share the placeholders
    auto f2 = std::bind(f, _3, std::bind(g, _3), _3, 4, 5);
    f2(10, 11, 12); // makes a call to f(12, g(12), 12, 4, 5);

    //显示如何处理随机数
    // common use case: binding a RNG with a distribution
    std::default_random_engine e;
    std::uniform_int_distribution<> d(0, 10);
    auto rnd = std::bind(d, e); // a copy of e is stored in rnd
    for (int n = 0; n < 10; ++n)
        std::cout << rnd() << ' ';
    std::cout << '\n';

    // bind to a pointer to member function
    Foo foo;
    auto f3 = std::bind(&Foo::print_sum, &foo, 95, _1);
    f3(5);

    // bind to a pointer to data member
    auto f4 = std::bind(&Foo::data, _1);
    std::cout << f4(foo) << '\n';

    // smart pointers can be used to call members of the referenced objects, too
    std::cout << f4(std::make_shared<Foo>(foo)) << '\n'
        << f4(std::make_unique<Foo>(foo)) << '\n';
    return 0;
}

//包装方法展示
class Test_BindA
{
public:

    //显然模版可以提供更大的灵活性，runfun 是函数
    template <typename runfun>
    int run_template_fun(runfun run)
    {
        std::cout << __func__ << std::endl;
        return run();
    }

    //适配方法展示,这反而麻烦了。
    template <typename runfun, typename arg1_type, typename arg2_type>
    int run_two_arg(runfun run, arg1_type arg1, arg2_type arg2)
    {
        std::cout << __func__ << std::endl;
        return run(arg1, arg2);
    }

    int run_function(std::function<int(int)>& run_func, int arg1)
    {
        std::cout << __func__ << std::endl;
        return run_func(arg1);
    }
};

int outer_fun1(int v1, int v2)
{
    std::cout << "v1=" << v1 << std::endl;
    std::cout << "v2=" << v2 << std::endl;
    return 0;
}

int outer_fun2(int v1, int v2, int v3)
{
    std::cout << "v1=" << v1 << std::endl;
    std::cout << "v2=" << v2 << std::endl;
    std::cout << "v3=" << v3 << std::endl;
    return 0;
}

int test_bind_2(int argc, char* argv[])
{
    // for _1, _2, _3...
    using namespace std::placeholders;
    Test_BindA tb;
    tb.run_template_fun(std::bind(outer_fun1, 1, 2));
    tb.run_template_fun(std::bind(outer_fun2, 1, 2, 3));
    auto lambda1 = [](int v1) -> int
    {
        std::cout << "lambda func" << std::endl;
        std::cout << "v1=" << v1 << std::endl;
        return 0;
    };
    tb.run_template_fun(std::bind(lambda1, 12));
    std::cout << "-----------------------------------------" << std::endl;

    //注意顺序
    tb.run_two_arg(std::bind(outer_fun2, 12, _1, _2), 3, 4);
    tb.run_two_arg(std::bind(outer_fun2, 12, _2, _1), 3, 4);

    std::cout << "-----------------------------------------" << std::endl;
    std::function<int(int)> f1 = std::bind(outer_fun1, 1024, _1);
    //注意第一个参数是引用
    tb.run_function(f1, 2048);
    std::cout << "-----------------------------------------" << std::endl;
    std::function<int(int)> f2_1 = std::bind(outer_fun2, _1, 1024, 3072);
    tb.run_function(f2_1, 2048);
    std::function<int(int)> f2_2 = std::bind(outer_fun2, 1024, _1, 3072);
    tb.run_function(f2_2, 2048);

    return 0;
}

#include <iostream>
using namespace std;

void global_func(int a)
{//全局函数
    cout << "call global_func:" << a << endl;
}

auto labmda = [](int a)
{
    cout << "lambda:" << a << endl;
};

class ClassA
{
public:
    void member_func(int a)
    {//类成员函数
        cout << "call ClassA::member_func:" << a << endl;
    }

    static void static_member_func(int a)
    {//类静态函数
        cout << "call ClassA::static_member_func:" << a << endl;
    }
};

class Functor
{//仿函数
public:
    void operator()(int a)
    {
        cout << "call Functor()" << a << endl;
    }
};

class CA
{
public:

    CA(int a, int b)
    {
        a_ = a;
        b_ = b;
    }

    //CA(const CA&) = default;
    ~CA() = default;

private:
    int a_ = 1;
    int b_ = 2;
};

int test_bind_3(int argc, char* argv[])
{
    std::function<void(int)> func;
    func = global_func;
    func(10);
    auto bindGlobalFunc = std::bind(global_func, 10);
    bindGlobalFunc();

    func = labmda;
    func(11);
    auto bindLabmdaFunc = std::bind(labmda, 11);
    bindLabmdaFunc();

    Functor testFunctor;
    func = testFunctor;
    func(12);
    auto bindFunctorFunc = std::bind(testFunctor, 12);
    bindFunctorFunc();

    ClassA a_object;
    func = std::bind(&ClassA::member_func, &a_object, std::placeholders::_1);
    func(13);
    auto bindClassMemberFunc = std::bind(&ClassA::member_func, &a_object, 13);
    bindClassMemberFunc();

    func = std::bind(&ClassA::static_member_func, std::placeholders::_1);
    func(14);
    auto bindClassStaticFunc = std::bind(&ClassA::static_member_func, 14);
    bindClassStaticFunc();

    //auto con_func = std::bind(&CA::CA);

    return 0;
}

/*
明白了这点，接下来就简单了。

声明：与普通函数作为区分，指向类的成员函数的指针只需要在指针前加上类类型即可，格式为：

typedef 返回值 (类名::*指针类型名)(参数列表);

赋值：只需要用类的成员函数地址赋值即可，格式为：

指针类型名  指针名 = &类名::成员函数名;

注意：这里的这个&符号是比较重要的：不加&，编译器会认为是在这里调用成员函数，所以需要给出参数列表，否则会报错；加了&，才认为是要获取函数指针。这是C++专门做了区别对待。

调用：调用方法也很简单，针对调用的对象是对象还是指针，分别用.*和->*进行调用，格式为：

(类对象.*指针名)(参数列表);

(类指针->*指针名)(参数列表);

*/
class HA
{
    HA() = default;
    HA(const HA&) = default;
    ~HA() = default;
    int a_ = 1;
    int b_ = 2;

public:
    size_t hash_key() const
    {
        return a_ + b_;
    }
    bool operator == (const HA& other)
    {
        if (other.a_ == this->a_ && other.b_ == this->b_)
        {
            return true;
        }
        return false;
    }
};

//================================================================

template<typename F>
class Test001
{
public:
    typedef std::function<F> func_type;

    Test001(const func_type& fn)
        : _fn(fn)
    {}

    template<typename ...Args>
    auto operator ()(Args&& ...args)
    {
        return _fn(std::forward<Args>(args)...);
    }

private:
    func_type _fn;
};

int test_function_001(int argc, char* argv[])
{
    std::function<void(const std::string&)> print = [](const std::string& name) {
        std::cout << "Hello " << name << " !" << std::endl;
    };

    Test001<void(const std::string&)> t(print);

    t("Jason");

    return 0;
}

class Test002
{
public:

    Test002()
    {
    }
    ~Test002()
    {
    }

    template<typename Fn, typename ...Args>
    int activate(Fn&& fn, Args&& ...args)
    {
        std::function<Fn> func;
        func = std::bind(*fn);
        //func();
        return 0;
    }

    template<typename Fn>
    int activate001(std::function<Fn>&& fn)
    {
        std::function<Fn> func01;
        //func01 = std::bind(fn);
        return 0;
    }

    int a_002_ = 0;
};