#include <random>
#include <iostream>
#include <memory>
#include <functional>

void f(int n1,int n2,int n3,const int& n4,int n5)
{
    std::cout << n1 << ' ' << n2 << ' ' << n3 << ' ' << n4 << ' ' << n5 << '\n';
}

int g(int n1)
{
    return n1;
}

struct Foo {
    void print_sum(int n1,int n2)
    {
        std::cout << n1 + n2 << '\n';
    }
    int data = 10;
};

int test_bind()
{
    using namespace std::placeholders;  // for _1, _2, _3...

    // demonstrates argument reordering and pass-by-reference
    int n = 7;
    // (_1 and _2 are from std::placeholders, and represent future
    // arguments that will be passed to f1)
    auto f1 = std::bind(f,_2,_1,42,std::cref(n),n);
    n = 10;
    f1(1,2,1001); // 1 is bound by _1, 2 is bound by _2, 1001 is unused
                    // makes a call to f(2, 1, 42, n, 7)

    // nested bind subexpressions share the placeholders
    auto f2 = std::bind(f,_3,std::bind(g,_3),_3,4,5);
    f2(10,11,12); // makes a call to f(12, g(12), 12, 4, 5);

    // common use case: binding a RNG with a distribution
    std::default_random_engine e;
    std::uniform_int_distribution<> d(0,10);
    auto rnd = std::bind(d,e); // a copy of e is stored in rnd
    for (int n = 0; n < 10; ++n)
        std::cout << rnd() << ' ';
    std::cout << '\n';

    // bind to a pointer to member function
    Foo foo;
    auto f3 = std::bind(&Foo::print_sum,&foo,95,_1);
    f3(5);

    // bind to a pointer to data member
    auto f4 = std::bind(&Foo::data,_1);
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

    //显然模版可以提供更大的灵活性
    template <typename runfun>
    int run_template_fun(runfun run)
    {
        std::cout << __func__ << std::endl;
        return run();
    }

    //适配方法展示,这反而麻烦了。
    template <typename runfun,typename arg1_type,typename arg2_type>
    int run_two_arg(runfun run,arg1_type arg1,arg2_type arg2)
    {
        std::cout << __func__ << std::endl;
        return run(arg1,arg2);
    }

    int run_function(std::function<int(int)>& run_func,int arg1)
    {
        std::cout << __func__ << std::endl;
        return run_func(arg1);
    }

};

int outer_fun1(int v1,int v2)
{
    std::cout << "v1=" << v1 << std::endl;
    std::cout << "v2=" << v2 << std::endl;
    return 0;
}

int outer_fun2(int v1,int v2,int v3)
{
    std::cout << "v1=" << v1 << std::endl;
    std::cout << "v2=" << v2 << std::endl;
    std::cout << "v3=" << v3 << std::endl;
    return 0;
}

int test_bind_2(int argc,char* argv[])
{
    // for _1, _2, _3...
    using namespace std::placeholders;  
    Test_BindA tb;
    tb.run_template_fun(std::bind(outer_fun1,1,2));
    tb.run_template_fun(std::bind(outer_fun2,1,2,3));
    auto lambda1 = [](int v1) -> int
    {
        std::cout << "lambda func" << std::endl;
        std::cout << "v1=" << v1 << std::endl;
        return 0;
    };
    tb.run_template_fun(std::bind(lambda1,12));
    std::cout << "-----------------------------------------" << std::endl;

    //注意顺序
    tb.run_two_arg(std::bind(outer_fun2,12,_1,_2),3,4);
    tb.run_two_arg(std::bind(outer_fun2,12,_2,_1),3,4);

    std::cout << "-----------------------------------------" << std::endl;
    std::function<int(int)> f1 = std::bind(outer_fun1,1024,_1);
    //注意第一个参数是引用
    tb.run_function(f1,2048);
    std::cout << "-----------------------------------------" << std::endl;
    std::function<int(int)> f2_1 = std::bind(outer_fun2,_1,1024,3072);
    tb.run_function(f2_1,2048);
    std::function<int(int)> f2_2 = std::bind(outer_fun2,1024,_1,3072);
    tb.run_function(f2_2,2048);


    return 0;
}