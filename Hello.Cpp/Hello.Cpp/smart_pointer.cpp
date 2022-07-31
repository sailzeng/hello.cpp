
#define _SILENCE_CXX17_SHARED_PTR_UNIQUE_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <memory>
#include <cstdio>
#include <fstream>
#include <cassert>
#include <functional>

#include <stdio.h>


//auto_ptr�����⡣
//1. ����auto_ptrָ��ͬһ���ڴ棬��ɶ���ͷ�
//2.������ɺ󣬻Ὣ���ƵĶ����ÿգ���˲��ܼ���ʹ��
//��Ϊ���������Σ��
//�������ݺ�Σ��




//share_ptr �Ľṹ
//https://blog.csdn.net/jxianxu/article/details/72858536
//https://blog.csdn.net/Xiejingfa/article/details/50750037

//https://heleifz.github.io/14696398760857.html


class Person
{
public:
    Person(int v) {
        value_=v;
        std::cout<<"Cons"<<value_<<std::endl;
    }
    ~Person() {
        std::cout<<"Des"<<value_<<std::endl;
    }

    int value_;

};

int hello_share_ptr_1(int argc,char* argv[])
{
    // Person(1)�����ü���Ϊ1
    std::shared_ptr<Person> p1(new Person(1));

    std::shared_ptr<Person> p2= std::make_shared<Person>(2);

    // ���������¶���Ȼ�����ü�����1�����ü���Ϊ0��������Person(1)
    // ����¶����ָ�뽻������ָ��
    p1.reset(new Person(3));

    //����p1��p3ͬʱָ��Person(3)��Person(3)�����ü���Ϊ2
    std::shared_ptr<Person> p3=p1;

    //Person(3)�����ü���Ϊ1
    p1.reset();
    //Person(3)�����ü���Ϊ0������Person(3)
    p3.reset();



    return 0;
}

/*
��Ͷ�ʴ�����Handler�У���this��Ϊ��������vector��emplace_back����������������ʵ�̲��˷��գ���������ڴ����е������ط���һ��shared_ptrҲָ�������Ͷ�ʶ��������ͻ����������ƿ鱻������������ô����û��һ�ְ취�������ǿ���ʹ��thisָ�빹��shared_ptr�������ֲ���������ƿ���ظ������أ�
���˵��ǣ���STL����һ��ģ��������Ϊ�˶�����������std::enable_shared_from_this������Ҫ�ﵽ���Ŀ�ģ�����̳�std::enable_shared_from_this�࣬����thisָ����shared_from_this()�ӿڴ��棬��������Ĵ���һ����
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
    //����return this��
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
        pf1=pf2->getFoo();  // �� pf2 �Ķ���������Ȩ
    }

    std::cout<<"pf2 is gone\n";

    return 0;
}

int hello_share_ptr_2(int argc,char* argv[])
{

    std::shared_ptr<int> p1_1(new int(0));
    std::shared_ptr<int> p1_2(p1_1);

    //unique ������C++17�ϳ��ˡ�

    //�������ʹ��unique ���������Կ������� unique ������false
    std::cout<<"p1_1 use count: "<<p1_1.use_count()<<std::endl;
    std::cout<<"p1_2 use count: "<<p1_2.use_count() <<std::endl;

    //error : ������ʽ����ȷ
    //std::shared_ptr<int> p_2(new int[10]);
    std::shared_ptr<int> p2_1(new int[10],[](int* p) { delete[]p; });

    int *tem_p3=new int(0);
    std::shared_ptr<int> p3_1(tem_p3);
    
    //error:�����
    std::shared_ptr<int> p3_2(tem_p3);

    //�������ʹ��unique ���������Կ������� unique ������true
    std::cout<<"p3_1 use count: "<<p3_1.use_count()<<std::endl;
    std::cout<<"p3_2 use count: "<<p3_2.use_count()<<std::endl;
    //����� 1��1��
    //p3_1 �� p3_2 ����Ա���һ��tem_p3��������һ��Control Block
    
    //����ʹ��make_shared

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


//��α��⽻������
class ClassB;
class ClassA
{
public:
    ClassA() 
    { 
        std::cout<<"ClassA Constructor..."<<std::endl;
    }
    ~ClassA() 
    {
        std::cout<<"ClassA Destructor..."<<std::endl;
    }
    // ��A������B
    std::weak_ptr<ClassB> pb_;
    //������� shared_ptr<ClassB> pb_;  �ͻ�����ڴ�й©
};

class ClassB
{
public:
    ClassB() 
    { 
        std::cout<<"ClassB Constructor..."<<std::endl; 
    }
    ~ClassB() 
    { 
        std::cout<<"ClassB Destructor..."<<std::endl; 
    }
    // ��B������A
    std::weak_ptr<ClassA> pa_;  
    //������� shared_ptr<ClassA> pa_;  �ͻ�����ڴ�й©
};

int hello_weak_ptr_2(int argc,char* argv[])
{
    std::shared_ptr<ClassA> spa=std::make_shared<ClassA>();
    std::shared_ptr<ClassB> spb=std::make_shared<ClassB>();
    spa->pb_=spb;
    spb->pa_=spa;
    // ����������˼��һ�£�spa��spb���ͷ���Դô��
    return 0;
}


//unique_ptr

int hello_unique_ptr(int argc,char* argv[])
{

    std::unique_ptr<int> up_p1(new int(1024));
    //�����
    //std::unique_ptr<int> one(up_p1); 
    //�����
    //std::unique_ptr<int> two=up_p1; 
    //C++ 14
    auto up_p2 =std::make_unique<int>(1024);

    //���Թ�����ֵ
    std::unique_ptr<int[]> up_p3(new int[1024]{0});

    return 0;
}


struct Up_B
{
    virtual void bar() 
    { 
        std::cout<<"B::bar\n"; 
    }
    virtual ~Up_B()=default;
};
struct Up_D: Up_B
{
    Up_D() 
    {
        std::cout<<"D::D\n";
    }
    ~Up_D() 
    {
        std::cout<<"D::~D\n";
    }
    void bar() override 
    {
        std::cout<<"D::bar\n";
    }
};

// a function consuming a unique_ptr can take it by value or by rvalue reference
std::unique_ptr<Up_D> pass_through(std::unique_ptr<Up_D> p)
{
    p->bar();
    return p;
}

void close_file(std::FILE* fp) 
{ 
    std::fclose(fp); 
}

int test_unique_ptr_2(int argc,char* argv[])
{
    std::cout<<"unique ownership semantics demo\n";
    {
        auto p=std::make_unique<Up_D>(); // p is a unique_ptr that owns a D
        auto q=pass_through(std::move(p));
        assert(!p); // now p owns nothing and holds a null pointer
        q->bar();   // and q owns the D object
    } // ~D called here

    //��η����������Լ���̬
    std::cout<<"Runtime polymorphism demo\n";
    {
        std::unique_ptr<Up_B> p=std::make_unique<Up_D>(); // p is a unique_ptr that owns a D
                                                      // as a pointer to base
        p->bar(); // virtual dispatch

        std::vector<std::unique_ptr<Up_B>> v;  // unique_ptr can be stored in a container
        v.push_back(std::make_unique<Up_D>());
        v.push_back(std::move(p));
        v.emplace_back(new Up_D);
        for(auto& p:v) 
            p->bar(); 
        // virtual dispatch
    } // ~D called 3 times

    std::cout<<"Custom deleter demo\n";
    std::ofstream("demo.txt")<<'x'; // prepare the file to read
    {
        std::unique_ptr<std::FILE,decltype(&close_file)> fp(fopen("demo.txt","r"),
                                                            &close_file);
        if(fp) // fopen could have failed; in which case fp holds a null pointer
            std::cout<<(char)std::fgetc(fp.get())<<'\n';
    } // fclose() called here, but only if FILE* is not a null pointer
      // (that is, if fopen succeeded)

    std::cout<<"Custom lambda-expression deleter demo\n";
    {
        std::unique_ptr<Up_D,std::function<void(Up_D*)>> p(new Up_D,[](Up_D* ptr)
                                                     {
                                                         std::cout<<"destroying from a custom deleter...\n";
                                                         delete ptr;
                                                     });  // p owns D
        p->bar();
    } // the lambda above is called and D is destroyed

    std::cout<<"Array form of unique_ptr demo\n";
    {
        std::unique_ptr<Up_D[]> p{new Up_D[3]};
    } // calls ~D 3 times

    return 0;
}