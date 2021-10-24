#include <iostream>
#include <string>

using namespace std;

void Throw() { throw 1; }

void NoBlockThrow() { Throw(); }

void BlockThrow() noexcept { Throw(); }

int hello_noexpect_1(int argc,char* argv[])
{
    try
    {
        Throw();
    }
    catch(...)
    {
        cout<<"Found throw."<<endl;     // Found throw.
    }

    try
    {
        NoBlockThrow();
    }
    catch(...)
    {
        cout<<"Throw is not blocked."<<endl;    // Throw is not blocked.
    }

    try
    {
        BlockThrow();   // terminate called after throwing an instance of 'int'
    }
    catch(...)
    {
        cout<<"Found throw 1."<<endl;
    }
    return 0;
}

//ע������ط�
template <class T> void noexpect_fun() noexcept(noexcept(T()))
{
    return;
};




struct NoExpect_A
{
    void test() { throw 1; }
};

struct NoExpect_B
{
    ~NoExpect_B() noexcept(false) { throw 2; }
};

struct NoExpect_C
{
    NoExpect_B b;
};

int funA()
{
    NoExpect_A a; a.test(); return 0;
}
int funB()
{
    NoExpect_B b; return 0;
}
int funC()
{
    NoExpect_C c; return 0;
}



int hello_noexpect_2(int argc,char* argv[])
{
    try
    {
        funB();
    }
    catch(...)
    {
        cout<<"caught funB."<<endl; // caught funB.
    }

    try
    {
        funC();
    }
    catch(...)
    {
        cout<<"caught funC."<<endl; // caught funC.
    }

    try
    {
        funA(); // terminate called after throwing an instance of 'int'
    }
    catch(...)
    {
        cout<<"caught funA."<<endl;
    }
    return 0;
}


struct Object
{
    virtual void fun()=0;
};

struct Base: public Object
{
    // ����Ϊfinal��������������
    void fun() final
    {
    };
};

struct Derived: public Base
{
    //void fun();     // �޷�ͨ������
};


struct Override_Base
{
    virtual void Turing()=0;
    virtual void Dijkstra()=0;
    virtual void VNeumann(int g)=0;
    virtual void DKnuth() const;
    void Print();
};

struct DerivedMid: public Override_Base
{
    // void VNeumann(double g);
    // �ӿڱ������ˣ������һ���汾��VNeumann����
};

struct DerivedTop: public DerivedMid
{
    void Turing() override;
    //void Dikjstra() override;           // �޷�ͨ�����룬ƴд���󣬲�������
    void Dijkstra() override;

    //void VNeumann(double g) override;   // �޷�ͨ�����룬������һ�£���������
    void VNeumann(int g) override;

    //void DKnuth() override;             // �޷�ͨ�����룬�����Բ�һ�£���������
    void DKnuth() const override ;             // �޷�ͨ�����룬�����Բ�һ�£���������

    //void Print() override;              // �޷�ͨ�����룬���麯������
};