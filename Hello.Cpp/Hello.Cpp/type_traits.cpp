#include <iostream>
#include <type_traits>

typedef std::decay<int>::type         A;                                     // A is int
typedef std::decay<int &>::type       B;                                     // B is int
typedef std::decay<int &&>::type      C;                                     // C is int
typedef std::decay<const int &>::type D;                                     // D is int
typedef std::decay<int[2]>::type      E;                                     // E is int *
typedef std::decay<int(int)>::type    F;                                     // F is int(*)(int)

class MyClass {};

typedef std::decay<MyClass>::type         MyA;                                 // A is MyClass
typedef std::decay<MyClass &>::type       MyB;                                 // B is MyClass
typedef std::decay<MyClass &&>::type      MyC;                                 // C is MyClass
typedef std::decay<const MyClass &>::type MyD;                                 // D is MyClass
typedef std::decay<MyClass[2]>::type      MyE;                                 // E is MyClass *
typedef std::decay<MyClass *>::type       MyF;                                 // E is MyClass *
typedef std::decay<MyClass *[2]>::type    MyG;                                 // G is MyClass **
typedef std::decay<MyClass **>::type      MyH;                                 // H is MyClass **

int hello_decay()
{
    std::cout << std::boolalpha;
    std::cout << std::is_same<int, A>::value << std::endl;                     // true
    std::cout << std::is_same<int, B>::value << std::endl;                     // true
    std::cout << std::is_same<int, C>::value << std::endl;                     // true
    std::cout << std::is_same<int, D>::value << std::endl;                     // true
    std::cout << std::is_same<int *, E>::value << std::endl;                     // true
    std::cout << std::is_same<int(int), F>::value << std::endl;                     // false
    std::cout << std::is_same<int(*)(int), F>::value << std::endl;                     // true

    std::cout << std::boolalpha;
    std::cout << std::is_same<MyClass, MyA>::value << std::endl;                      // true
    std::cout << std::is_same<MyClass, MyB>::value << std::endl;                      // true
    std::cout << std::is_same<MyClass, MyC>::value << std::endl;                      // true
    std::cout << std::is_same<MyClass, MyD>::value << std::endl;                      // true
    std::cout << std::is_same<MyClass *, MyE>::value << std::endl;                      // true
    std::cout << std::is_same<MyClass *, MyF>::value << std::endl;                      // true
    std::cout << std::is_same<MyClass **, MyG>::value << std::endl;                      // true
    std::cout << std::is_same<MyClass **, MyH>::value << std::endl;                      // true

    return 1;
}