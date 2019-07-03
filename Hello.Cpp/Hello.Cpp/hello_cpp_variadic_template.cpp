

#include <iostream>


template < typename ... T>
void dummy_wrapper(T... t)
{
};

template < class T>
T unpacker(const T t)
{
    std::cout << '[' << t << ']';
    return t;
}


template < typename... Args>
void write_line(const Args& ... data)
{
    dummy_wrapper(unpacker(data)...);
    std::cout << '\n';
}

int test_variadic(int argc,char* argv[])
{
    write_line(1,"--","2.2.2","--",3.0);
    return 0;
}


template <typename ... Tlist>
void dummy_wrapper(Tlist... tlst)
{
};

template <typename T,typename ... Tlist>
void dummy_wrapper(T t,Tlist... tlst)
{
    unpacker(t);
    dummy_wrapper(tlst...);
};

template <class T>
T unpacker(const T t)
{
    std::cout << '[' << t << ']';
    return t;
}

template <typename... Args>
void write_line(const Args& ... data)
{
    dummy_wrapper(data...);
    std::cout << '\n';
}

int test_variadic2(int argc,char* argv[])
{
    write_line(1,"--","2.2.2","--",3.0);
    return 0;
}