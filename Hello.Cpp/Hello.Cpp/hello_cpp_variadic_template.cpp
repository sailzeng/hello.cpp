

#include <iostream>


template <typename T,typename... TList>
void dummy_wrapper(T t,TList... tlist)
{
};

template < class T>
T unpacker(const T t)
{
    std::cout << '[' << t << ']';
    return t;
}

//
template <typename Arg, typename... ArgsList>
void write_line(const Arg &data,const ArgsList& ... datalist)
{
    dummy_wrapper(unpacker(data),unpacker(datalist)...);
    std::cout << '\n';
}

int test_variadic(int argc,char* argv[])
{
    write_line(1,"--","2.2.2","--",3.0);
    return 0;
}

//----------------------------------------------------------------
template <class T>
T unpacker2(const T t)
{
    std::cout << '[' << t << ']';
    return t;
}

//ÊÊÅä0¸ö²ÎÊı
template <typename ... Tlist>
void dummy_wrapper2(Tlist... tlst)
{
};

template <typename T,typename ... Tlist>
void dummy_wrapper2(T t,Tlist... tlst)
{
    unpacker2(t);
    dummy_wrapper2(tlst...);
};

template <typename... Args>
void write_line2(const Args& ... data)
{
    dummy_wrapper2(data...);
    std::cout << '\n';
}

int test_variadic2(int argc,char* argv[])
{
    write_line2(1,"--","2.2.2","--",3.0);
    return 0;
}