#include <functional>
#include <queue>
#include <vector>
#include <iostream>



void process_value(int& i)
{
    std::cout<<"LValue processed: "<<i<<std::endl;
}

void process_value(int&& i)
{
    std::cout<<"RValue processed: "<<i<<std::endl;
}

void forward_value(int&& i)
{
    process_value(i);
}

int hello_rvalue_1(int argc,char* argv[])
{
    int a=0;
    process_value(a);
    process_value(1);
    //
    forward_value(2);

    return 0;
}



class Intvec
{
public:
    explicit Intvec(size_t num=0)
        : size_(num),data_(new int[size_])
    {
        log("constructor");
    }

    ~Intvec()
    {
        log("destructor");
        if(data_)
        {
            delete[] data_;
            data_=0;
        }
    }

    Intvec(const Intvec& other)
        : size_(other.size_),data_(new int[size_])
    {
        log("copy constructor");
        for(size_t i=0; i<size_; ++i)
            data_[i]=other.data_[i];
    }

    Intvec& operator=(const Intvec& other)
    {
        log("copy assignment operator");
        Intvec tmp(other);
        std::swap(size_,tmp.size_);
        std::swap(data_,tmp.data_);
        return *this;
    }

    //注意noexcept
    Intvec(Intvec&& other) noexcept:
        size_(0),
        data_(nullptr)
    {
        log("move constructor");
        std::swap(size_,other.size_);
        std::swap(data_,other.data_);
        //是不是置位NULL 更好？下面也有这个问题
        //size_ = other.size_;
        //data_ = other.data_;
        //other.size_ = 0;
        //other.data_ = nullptr;
    }
     
    Intvec& operator=(Intvec&& other) noexcept
    {
        log("move assignment operator");
        std::swap(size_,other.size_);
        std::swap(data_,other.data_);
        return *this;
    }

private:

    void log(const char* msg)
    {
        std::cout<<"["<<this<<"] "<<msg<<"\n";
    }

    size_t size_;
    int* data_;
};


//universal references(通用引用)

template<typename T>
void f1(T&& param)
{
    if(std::is_same<std::string,T>::value_)
        std::cout<<"string"<<std::endl;
    else if(std::is_same<std::string&,T>::value_)
        std::cout<<"string&"<<std::endl;
    else if(std::is_same<std::string&&,T>::value_)
        std::cout<<"string&&"<<std::endl;
    else if(std::is_same<int,T>::value_)
        std::cout<<"int"<<std::endl;
    else if(std::is_same<int&,T>::value_)
        std::cout<<"int&"<<std::endl;
    else if(std::is_same<int&&,T>::value_)
        std::cout<<"int&&"<<std::endl;
    else
        std::cout<<"unkown"<<std::endl;
}

int hello_universal_references(int argc,char* argv[])
{
    int x=1;
    f1(1); // 参数是右值 T推导成了int, 所以是int&& param, 右值引用
    f1(x); // 参数是左值 T推导成了int&, 所以是int&&& param, 折叠成 int&,左值引用
    int&& a=2;
    f1(a); //虽然a是右值引用，但它还是一个左值， T推导成了int&
    std::string str="hello";
    f1(str); //参数是左值 T推导成了string&
    f1(std::string("hello")); //参数是右值， T推导成了string
    f1(std::move(str));//参数是右值， T推导成了string

    return 0;
}

void RunCode(int&& m)
{
    std::cout<<"rvalue ref"<<std::endl;
}
void RunCode(int& m)
{
    std::cout<<"lvalue ref"<<std::endl;
}
void RunCode(const int&& m)
{
    std::cout<<"const rvalue ref"<<std::endl;
}
void RunCode(const int& m)
{
    std::cout<<"const lvalue ref"<<std::endl;
}

// 这里利用了universal references，如果写T&,就不支持传入右值，而写T&&，既能支持左值，又能支持右值
template<typename T>
void perfectForward(T&& t)
{
    //完美转发
    RunCode(std::forward<T>(t));
}

template<typename T>
void notPerfectForward(T&& t)
{
    RunCode(t);
}

int hello_perfect_forward(int argc,char* argv[])
{
    int a=0;
    int b=0;
    const int c=0;
    const int d=0;

    notPerfectForward(a); // lvalue ref
    notPerfectForward(std::move(b)); // lvalue ref
    notPerfectForward(c); // const lvalue ref
    notPerfectForward(std::move(d)); // const lvalue ref

    std::cout<<"-------------------------------"<<std::endl;
    perfectForward(a); // lvalue ref
    perfectForward(std::move(b)); // rvalue ref
    perfectForward(c); // const lvalue ref
    perfectForward(std::move(d)); // const rvalue ref

    return 0;
}
