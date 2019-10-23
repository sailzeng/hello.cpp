
#define _SILENCE_CXX17_SHARED_PTR_UNIQUE_DEPRECATION_WARNING

#include <memory>
#include <string>
#include <iostream>

//auto_ptr的问题。
//1. 两个auto_ptr指向同一块内存，造成多次释放
//2.复制完成后，会将复制的对象置空，因此不能继续使用
//作为容器对象很危险
//参数传递很危险




//share_ptr 的结构
//https://blog.csdn.net/jxianxu/article/details/72858536
//https://blog.csdn.net/Xiejingfa/article/details/50750037

//https://heleifz.github.io/14696398760857.html


class Person
{
public:
    Person(int v) {
        value=v;
        std::cout<<"Cons"<<value<<std::endl;
    }
    ~Person() {
        std::cout<<"Des"<<value<<std::endl;
    }

    int value;

};

int hello_share_ptr_1(int argc,char* argv[])
{
    // Person(1)的引用计数为1
    std::shared_ptr<Person> p1(new Person(1));

    std::shared_ptr<Person> p2= std::make_shared<Person>(2);

    // 首先生成新对象，然后引用计数减1，引用计数为0，故析构Person(1)
    // 最后将新对象的指针交给智能指针
    p1.reset(new Person(3));

    //现在p1和p3同时指向Person(3)，Person(3)的引用计数为2
    std::shared_ptr<Person> p3=p1;

    //Person(3)的引用计数为1
    p1.reset();
    //Person(3)的引用计数为0，析构Person(3)
    p3.reset();



    return 0;
}

/*
在投资处理函数Handler中，用this作为参数传给vector的emplace_back函数，但这样做其实蕴藏了风险，试想如果在代码中的其他地方有一个shared_ptr也指向了这个投资对象，这样就会有两个控制块被建立起来。那么，有没有一种办法，让我们可以使用this指针构造shared_ptr，但是又不会引起控制块的重复创建呢？
幸运的是，在STL中有一个模板类正是为此而生，它就是std::enable_shared_from_this。代码要达到这个目的，必须继承std::enable_shared_from_this类，并将this指针用shared_from_this()接口代替，就像下面的代码一样：
*/
struct SFT_Foo: public std::enable_shared_from_this<SFT_Foo>
{
    SFT_Foo() 
    { 
        std::cout<<"Foo::Foo\n"; 
    }
    ~SFT_Foo() 
    { std::cout<<"Foo::~Foo\n"; 
    }
    //不能return this，
    std::shared_ptr<SFT_Foo> getFoo() 
    { 
        return shared_from_this(); 
    }
};

int hello_share_from_this(int argc,char* argv[])
{
    SFT_Foo* f=new SFT_Foo;
    std::shared_ptr<SFT_Foo> pf1;

    {
        std::shared_ptr<SFT_Foo> pf2(f);
        pf1=pf2->getFoo();  // 与 pf2 的对象共享所有权
    }

    std::cout<<"pf2 is gone\n";

    return 0;
}

int hello_share_ptr_2(int argc,char* argv[])
{

    std::shared_ptr<int> p1_1(new int(0));
    std::shared_ptr<int> p1_2(p1_1);

    //unique 函数被C++17废除了。

    //如果可以使用unique 函数，可以看出下面 unique 返回了false
    std::cout<<"p1_1 use count: "<<p1_1.use_count()<<std::endl;
    std::cout<<"p1_2 use count: "<<p1_2.use_count() <<std::endl;

    //error : 析构方式不正确
    //std::shared_ptr<int> p_2(new int[10]);
    std::shared_ptr<int> p2_1(new int[10],[](int* p) { delete[]p; });

    int *tem_p3=new int(0);
    std::shared_ptr<int> p3_1(tem_p3);
    
    //error:会崩溃
    std::shared_ptr<int> p3_2(tem_p3);

    //如果可以使用unique 函数，可以看出下面 unique 返回了true
    std::cout<<"p3_1 use count: "<<p3_1.use_count()<<std::endl;
    std::cout<<"p3_2 use count: "<<p3_2.use_count()<<std::endl;
    //输出是 1，1，
    //p3_1 和 p3_2 会各自保持一个tem_p3，各自用一个Control Block
    
    //尽量使用make_shared

    return 0;
}

// weak_ptr
//https://blog.csdn.net/Xiejingfa/article/details/50772571
//https://blog.csdn.net/jxianxu/article/details/72859301#comments

int hello_weak_ptr(int argc,char* argv[])
{
    std::shared_ptr<int> sp_p1(new int(1024));
    std::shared_ptr<int> sp_p2(sp_p1);

    std::weak_ptr<int> wp_p1(sp_p1);

    if(auto sp_p3 =wp_p1.lock())
    {
        std::cout<<*sp_p3<<std::endl;
        std::cout<<"sp_p3 live!"<<sp_p3.use_count()<<std::endl;
        std::cout<<"WP expired?"<<wp_p1.expired()<<std::endl;
    }
    else
    {
        std::cout<<"sp_p3 is not exist!"<<std::endl;
    }

    sp_p1.reset();
    if(auto sp_p3=wp_p1.lock())
    {
        std::cout<<*sp_p3<<std::endl;
        std::cout<<"sp_p3 live!"<<sp_p3.use_count()<<std::endl;
        std::cout<<"WP expired?"<<wp_p1.expired()<<std::endl;
    }
    else
    {
        std::cout<<"sp_p1 is not exist!"<<std::endl;
    }

    sp_p2.reset();
    if(auto sp_p3=wp_p1.lock())
    {
        std::cout<<*sp_p3<<std::endl;
        std::cout<<"sp_p1 live!"<<std::endl;
    }
    else
    {
        std::cout<<"sp_p1 is not exist!"<<std::endl;
        std::cout<<"WP expired?"<<wp_p1.expired()<<std::endl;
    }
    return 0;
}




