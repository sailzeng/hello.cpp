
#include <memory>
#include <string>
#include <iostream>

//auto_ptr�����⡣
//1. ����auto_ptrָ��ͬһ���ڴ棬��ɶ���ͷ�
//2.������ɺ󣬻Ὣ���ƵĶ����ÿգ���˲��ܼ���ʹ��
//��Ϊ���������Σ��
//�������ݺ�Σ��


//share_ptr �Ľṹ
//https://blog.csdn.net/jxianxu/article/details/72858536
//




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

int hello_share_ptr(int argc,char* argv[])
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


    std::shared_ptr<int> p_1(new int(0));

    //error
    //std::shared_ptr<int> p_2(new int[10]);

    std::shared_ptr<int> p_2(new int[10],[](int *p) { delete []p; });

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

