
#include <any> 
#include <vector>
#include <variant>
#include <iostream> 
#include <string> 
#include <cassert>
#include <functional>
#include <optional>

//any是一个存放任意数据的实现。
using std::any;

int test_tuple_init(int argc,char* argv[])
{
    std::tuple <int,double> a;
    a={1,1.002};

    return 0;
}

#if 1
struct TEST_A
{
    int a_;
    int b_;
    int c_;
};

int test_any(int argc, char* argv[])
{
	try {

		// Integer 42:  Using the copy initialisation 
		std::any value_ = 42;
		std::cout << "Value: "
			<< std::any_cast<int>(value_) << std::endl;

		//bad_any_cast any并不能进行内部转换
		//std::cout << "Value: "
		//	<< std::any_cast<double>(value) << std::endl;

		// Using the assignment operator 
		// to store a string 
		value_ = "Hello World";
		std::cout << "Value: "
			<< std::any_cast<const char*>(value_) << std::endl;

		// Using the parametrized constructor 
		any val(19.0);
		std::cout << "Value: "
			<< std::any_cast<double>(val) << std::endl;
    	//bad_any_cast
		//std::cout << "Value: "
		//	<< std::any_cast<int>(val) << std::endl;

		any val_brace{std::string("Brace Initialisation")};
		std::cout << "Value: "
			<< std::any_cast<std::string>(val_brace) << std::endl;

		//any可以模仿动态类型语言的变量。注意，any不是模板类
		std::vector<std::any>  va;
        va.push_back(std::any(9.0));
        va.push_back(std::any(11));
	}
	//转换失败会抛出异常
	catch (std::bad_any_cast& e) {
		std::cout << "\n"
			<< e.what() << std::endl;
	}
	return 0;
}

//tuple将返回值灵活话了。
std::tuple<double, char, std::string> get_student(int id)
{
	if (id == 0) return std::make_tuple(3.8, 'A', "Lisa Simpson");
	if (id == 1) return std::make_tuple(2.9, 'C', "Milhouse Van Houten");
	if (id == 2) return std::make_tuple(1.7, 'D', "Ralph Wiggum");
	throw std::invalid_argument("id");
}

int test_tuple(int argc, char* argv[])
{
	auto student0 = get_student(0);
	std::cout << "ID: 0, "
		<< "GPA: " << std::get<0>(student0) << ", "
		<< "grade: " << std::get<1>(student0) << ", "
		<< "name: " << std::get<2>(student0) << '\n';

	double gpa1;
	char grade1;
	std::string name1;
	std::tie(gpa1, grade1, name1) = get_student(1);
	std::cout << "ID: 1, "
		<< "GPA: " << gpa1 << ", "
		<< "grade: " << grade1 << ", "
		<< "name: " << name1 << '\n';

	// C++17 结构化绑定：
	auto [gpa2, grade2, name2] = get_student(2);
	std::cout << "ID: 2, "
		<< "GPA: " << gpa2 << ", "
		<< "grade: " << grade2 << ", "
		<< "name: " << name2 << '\n';

	return 0;
}

//C++ 17
int test_variant(int argc, char* argv[])
{
	std::variant<int, float> v, w;
	v = 12; // v 含 int
	int i = std::get<int>(v);
	w = std::get<int>(v);
	w = std::get<0>(v); // 与前一行效果相同
	w = v; // 与前一行效果相同

	//  std::get<double>(v); // 错误： [int, float] 中无 double
	//  std::get<3>(v);      // 错误：合法下标值为 0 与 1


	try {
		float a = std::get<float>(w); // w 含 int 而非 float ：将抛出
	}
	catch (const std::bad_variant_access&) {
	}

	using namespace std::literals;

	std::variant<std::string> x("abc"); // 转换构造函数在无歧义时起作用
	x = "def"; // 转换赋值在无歧义时亦起作用

	std::variant<std::string, void const*> y("abc");
	// 传递 char const * 时转换成 void const *
	// 成功
	assert(std::holds_alternative<void const*>(y));
	y = "xyz"s;
	// 成功
	assert(std::holds_alternative<std::string>(y));

	return 0;
}

int test_variant2(int argc, char* argv[])
{
	std::variant<int, double, std::string> v;
	v = 1;
	bool has_int = std::holds_alternative<int>(v);
	bool has_double = std::holds_alternative<double>(v);

    // 01011
    std::cout << "index : " << v.index() << " " << has_int << " " << has_double << " " << std::endl;
    std::cout << std::get<0>(v) << " " << *std::get_if<0>(&v) << std::endl;
    std::cout << std::endl;
	v = 2.0;
	std::cout << v.index() << " " << (std::get_if<int>(&v) == nullptr) << std::get<1>(v) << std::get<double>(v) << std::endl; // 1122
    std::cout << std::endl;
	v = "a";
	std::cout << v.index() << " " << std::get<2>(v) << std::get<std::string>(v) << std::endl; // 2aa
    std::cout << std::endl;
	return 0;
}

// optional can be used as the return type of a factory that may fail
std::optional<std::string> create(bool b) {
    if (b)
        return "Godzilla";
    return {};
}

// std::nullopt can be used to create any (empty) std::optional
auto create2(bool b) {
    return b ? std::optional<std::string>{"Godzilla"} : std::nullopt;
}

// std::reference_wrapper may be used to return a reference
auto create_ref(bool b) {
    static std::string value_ = "Godzilla";
    return b ? std::optional<std::reference_wrapper<std::string>>{value_}
    : std::nullopt;
}

int test_optional(int argc, char* argv[])
{
    std::cout << "create(false) returned "
        << create(false).value_or("empty") << '\n';

    // optional-returning factory functions are usable as conditions of while and if
    //同时判定了str 是否有效。
    if (auto str = create2(true)) 
    {
        std::cout << "create2(true) returned " << *str << '\n';
    }

    std::cout << create2(false).value_();

    if (auto str = create_ref(true)) {
        // using get() to access the reference_wrapper's value
        std::cout << "create_ref(true) returned " << str->get() << '\n';
        str->get() = "Mothra";
        std::cout << "modifying it changed it to " << str->get() << '\n';
    }

    return 0;
}

#endif 



