#include <iostream>
#include <string>
#include <typeinfo>

using size_t = decltype(sizeof(0));
using ptrdiff_t = decltype((int*)0-(int*)0);
using nullptr_t = decltype(nullptr);

using namespace std;

//重用匿名类
enum { K1,K2,K3 }anon_e;   // 匿名的强类型枚举

union
{
    decltype(anon_e) key;
    char* name;
}anon_u;    // 匿名的union

struct
{
    int d;
    decltype(anon_u) id;
}anon_s[100];   // 匿名的struct数组


int test_decltype_1()
{
    decltype(anon_s) as;
    // 引用匿名强类型枚举中的值
    as[0].id.key=decltype(anon_e)::K1;   
}



int i=4;

decltype(i) a;      // a: int
//decltype((i)) b;    // b: int &, 无法编译通过

int arr[5]={0};
int* ptr=arr;

struct S { double d; } s;

void Overloaded(int);
void Overloaded(char);      // 重载的函数

int&& RvalRef();

const bool Func(int);

// 规则1: 单个标记符表达式以及访问类成员，推导为本类型
decltype(arr) var1;            // int[5], 标记符表达式
decltype(ptr) var2;            // int*, 标记符表达式
decltype(s.d) var4;            // double, 成员访问表达式

//decltype(Overloaded) var5;     // 无法通过编译，是个重载的函数

// 规则2: 将亡值，推导为类型的右值引用
decltype(RvalRef()) var6=1;  // int&&

// 规则3: 左值，推导为类型的引用
decltype(true?i:i) var7=i;    // int&, 三元运算符，这里返回一个i的左值
decltype((i)) var8=i;             // int&, 带圆括号的左值 
decltype(++i) var9=i;             // int&, ++i返回i的左值
decltype(arr[3]) var10=i;         // int& []操作返回左值
decltype(*ptr)  var11=i;          // int& *操作返回左值
decltype("lval") var12="lval";    // const char(&)[9], 字符串字面常量为左值

// 规则4：以上都不是，推导为本类型
decltype(1) var13;              // int, 除字符串外字面常量为右值
decltype(i++) var14;            // int, i++返回右值
decltype((Func(1))) var15;      // const bool, 圆括号可以忽略




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