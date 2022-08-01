

#include <iostream>
#include <string>
#include <tuple>
#include <iostream>
#include <array>
#include <utility>

template <typename... TList>
void dummy_wrapper(TList... tlist)
{
};

template < class T>
T unpacker(const T t)
{
    std::cout << '[' << t << ']';
    return t;
}

//
template <typename...ArgsList>
void write_line(const ArgsList& ...datalist)
{
    dummy_wrapper(unpacker(datalist)...);
    std::cout << '\n';
}

//����ĺ���������ǵߵ��ģ�������ΪVC++���ڲ���չ����˳���Ǵ�������
int test_variadic(int argc, char* argv[])
{
    std::cout << __func__ << std::endl;
    write_line(1, "--", "2.2.2", "--", 3.0);

    int i = 0;

    //ͨ����������Ĳ������ݺ����漸�����Ա�
    auto test_para = [](int a, int b, int c)
    {
        std::cout << "[" << a << "]--[" << b << "]--[" << c << "]" << std::endl;
    };
    i = 0;
    test_para(i++, i++, i++); //���[2]--[1]--[0]
    i = 0;
    test_para(++i, ++i, ++i); //���[3]--[2]--[1]

    //ע�������ĩβ������ʵ��0�����������֤���ˣ�
    //��ΪVC++���ҵ��󴫵ݣ�չ�������������ԡ�
    i = 0;
    write_line(i++, "--", i++, "--", i++); //[0][--][1][--][2]
    i = 0;
    write_line(i++, "--", i++, "--", i++ == 0 ? 100 : 101);//[100][--][1][--][2]

    i = 0;
    //��������Ϊʲô��3--3--3�أ���Ϊwrite_line�Ĳ����Զ�ƥ��Ϊ��const &
    write_line(++i, "--", ++i, "--", ++i); //[3][--][3][--][3]
    i = 0;
    write_line(std::to_string(++i),
               "--",
               std::to_string(++i),
               "--",
               std::to_string(++i)); //[1][--][2][--][3]

    return 0;
}

//----------------------------------------------------------------
template <class T>
T unpacker2(const T t)
{
    std::cout << '[' << t << ']';
    return t;
}

//����0������
template <typename ... Tlist>
void dummy_wrapper2(Tlist... tlst)
{
};

template <typename T, typename ... Tlist>
void dummy_wrapper2(T t, Tlist... tlst)
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

int test_variadic2(int argc, char* argv[])
{
    std::cout << __func__ << std::endl;
    write_line2(1, "--", "2.2.2", "--", 3.0);
    int i = 0;
    write_line2(i++, "--", i++, "--", i++);
    i = 0;
    write_line2(++i, "--", ++i, "--", ++i);
    i = 0;
    write_line2(std::to_string(++i), "--", std::to_string(++i), "--", std::to_string(++i));
    return 0;
}

//----------------------------------------------------------------
// Ư���ش�ӡ tuple

template<class Ch, class Tr, class Tuple, std::size_t... Is>
void print_tuple_impl(std::basic_ostream<Ch, Tr>& os,
                      const Tuple& t,
                      std::index_sequence<Is...>)
{
    ((os << (Is == 0 ? "" : ", ") << std::get<Is>(t)), ...);
}

template<class Ch, class Tr, class... Args>
auto& operator<<(std::basic_ostream<Ch, Tr>& os, const std::tuple<Args...>& t)
{
    os << "(";
    print_tuple_impl(os, t, std::index_sequence_for<Args...>{});
    return os << ")";
}

//----------------------------------------------------------------

template<typename FUNC, typename PARA, std::size_t... I>
auto call_impl(FUNC& f, const PARA& a, std::index_sequence<I...>)
{
    return f(std::get<I>(a)...);
}

template<typename FUNC, typename PARA>
auto call(FUNC& f, const PARA& a)
{
    static constexpr auto t_count = std::tuple_size<PARA>::value_;
    return call_impl(f, a, std::make_index_sequence<t_count>());
}

template <typename PARA>
void dummy_wrapper3(PARA tlist)
{
    std::cout << tlist << '\n';
};

template < class T>
T unpacker3(const T t)
{
    std::cout << '[' << t << ']';
    return t;
}

//
template <typename...ArgsList>
void write_line3(const ArgsList& ...datalist)
{
    auto t = std::make_tuple(unpacker3(datalist)...);
    dummy_wrapper3(t);
    std::cout << '\n';

    int i = 0;
    i = 0;
    auto a = std::make_tuple(i++, i++, i++);
    std::cout << a << '\n';
    i = 0;
    auto b = std::make_tuple(++i, ++i, ++i);
    std::cout << b << '\n';
    i = 0;
    auto d = std::make_tuple(std::to_string(i++),
                             std::to_string(i++),
                             std::to_string(i++));
    std::cout << d << '\n';

    //VC++Ŀǰ����֧�������ʼ����ʽ
    //std::tuple<ArgsList...> c = {datalist...};
    //std::cout << c << '\n';
}

int test_variadic3(int argc, char* argv[])
{
    std::cout << __func__ << std::endl;
    write_line3(1, "--", "2.2.2", "--", 3.0);
    return 0;
}

//----------------------------------------------------------------


// ת������Ϊ tuple
template<typename Array, std::size_t... I>
auto a2t_impl(const Array& a, std::index_sequence<I...>)
{
    return std::make_tuple(a[I]...);
}

template<typename T, std::size_t N, typename Indices = std::make_index_sequence<N>>
auto a2t(const std::array<T, N> & a)
{
    return a2t_impl(a, Indices{});
}



int test_index_sequence(int argc, char* argv[])
{
    std::array<int, 4> array = { 1,2,3,4 };

    // ת�� array Ϊ tuple
    auto tuple = a2t(array);
    static_assert(std::is_same<decltype(tuple),
                  std::tuple<int, int, int, int>>::value, "");

    // ��ӡ�� cout
    std::cout << tuple << '\n';
    return 0;
}

//----------------------------------------------------------------

template <typename... TList>
int dummy_wrapper4(TList... tlist)
{
    return 0;
};

template < class T>
T unpacker4(const T t)
{
    std::cout << '[' << t << ']';
    return t;
}

//
template <typename...ArgsList>
void write_line4(const ArgsList& ...datalist)
{
    int arr[] = { dummy_wrapper4(unpacker4(datalist))... };
    std::cout << '\n';
}

//����ĺ���������ǵߵ��ģ�������ΪVC++���ڲ���չ����˳���Ǵ�������
int test_variadic4(int argc, char* argv[])
{
    write_line4(1, "--", "2.2.2", "--", 3.0);
    int i = 0;
    write_line4(i++, i++, i++, i++, i++);
    return 0;
}


//----------------------------------------------------------------

template <typename... TList>
void dummy_wrapper5(TList... tlist)
{
};

template < class T>
T unpacker5(const T t)
{
    std::cout << '[' << t << ']';
    return t;
}

//
template <typename...ArgsList>
void write_line5(const ArgsList& ...datalist)
{
    int arr[] = { (dummy_wrapper4(unpacker4(datalist)),0)... };
    std::cout << '\n';
}

//����ĺ���������ǵߵ��ģ�������ΪVC++���ڲ���չ����˳���Ǵ�������
int test_variadic5(int argc, char* argv[])
{
    std::cout << __func__ << std::endl;
    write_line5(1, "--", "2.2.2", "--", 3.0);
    int i = 0;
    write_line5(i++, i++, i++, i++, i++);
    return 0;
}




