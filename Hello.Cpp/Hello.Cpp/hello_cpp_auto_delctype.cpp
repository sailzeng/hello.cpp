#include <iostream>
#include <string>
#include <typeinfo>

using size_t = decltype(sizeof(0));
using ptrdiff_t = decltype((int*)0-(int*)0);
using nullptr_t = decltype(nullptr);

using namespace std;

//����������
enum { K1,K2,K3 }anon_e;   // ������ǿ����ö��

union
{
    decltype(anon_e) key;
    char* name;
}anon_u;    // ������union

struct
{
    int d;
    decltype(anon_u) id;
}anon_s[100];   // ������struct����


int test_decltype_1()
{
    decltype(anon_s) as;
    // ��������ǿ����ö���е�ֵ
    as[0].id.key=decltype(anon_e)::K1;   
}



int i=4;

decltype(i) a;      // a: int
//decltype((i)) b;    // b: int &, �޷�����ͨ��

int arr[5]={0};
int* ptr=arr;

struct S { double d; } s;

void Overloaded(int);
void Overloaded(char);      // ���صĺ���

int&& RvalRef();

const bool Func(int);

// ����1: ������Ƿ����ʽ�Լ��������Ա���Ƶ�Ϊ������
decltype(arr) var1;            // int[5], ��Ƿ����ʽ
decltype(ptr) var2;            // int*, ��Ƿ����ʽ
decltype(s.d) var4;            // double, ��Ա���ʱ��ʽ

//decltype(Overloaded) var5;     // �޷�ͨ�����룬�Ǹ����صĺ���

// ����2: ����ֵ���Ƶ�Ϊ���͵���ֵ����
decltype(RvalRef()) var6=1;  // int&&

// ����3: ��ֵ���Ƶ�Ϊ���͵�����
decltype(true?i:i) var7=i;    // int&, ��Ԫ����������ﷵ��һ��i����ֵ
decltype((i)) var8=i;             // int&, ��Բ���ŵ���ֵ 
decltype(++i) var9=i;             // int&, ++i����i����ֵ
decltype(arr[3]) var10=i;         // int& []����������ֵ
decltype(*ptr)  var11=i;          // int& *����������ֵ
decltype("lval") var12="lval";    // const char(&)[9], �ַ������泣��Ϊ��ֵ

// ����4�����϶����ǣ��Ƶ�Ϊ������
decltype(1) var13;              // int, ���ַ��������泣��Ϊ��ֵ
decltype(i++) var14;            // int, i++������ֵ
decltype((Func(1))) var15;      // const bool, Բ���ſ��Ժ���




int test_decltype_2()
{
    cout<<is_rvalue_reference<decltype(RvalRef())>::value<<endl;    // 1

    cout<<is_lvalue_reference<decltype(true?i:i)>::value<<endl; // 1
    cout<<is_lvalue_reference<decltype((i))>::value<<endl;          // 1
    cout<<is_lvalue_reference<decltype(++i)>::value<<endl;          // 1
    cout<<is_lvalue_reference<decltype(arr[3])>::value<<endl;       // 1
    cout<<is_lvalue_reference<decltype(*ptr)>::value<<endl;         // 1
    cout<<is_lvalue_reference<decltype("lval")>::value<<endl;       // 1

    cout<<is_lvalue_reference<decltype(i++)>::value<<endl;          // 0
    cout<<is_rvalue_reference<decltype(i++)>::value<<endl;          // 0
}