
#include <memory>
#include <string>
#include <iostream>

class TestSmartPtr1
{
public:
	TestSmartPtr1(std::string s)
	{
		str = s;
		std::cout << "Test creat\n";
	}
	~TestSmartPtr1()
	{
		std::cout << "Test delete:" << str << std::endl;
	}
	std::string& getStr()
	{
		return str;
	}
	void setStr(std::string s)
	{
		str = s;
	}
	void print()
	{
		std::cout << str << std::endl;
	}
private:
	std::string str;
};

std::unique_ptr<TestSmartPtr1> fun()
{
	return std::unique_ptr<TestSmartPtr1>(new TestSmartPtr1("789"));
}





class TestSmartPtr2
{
public:
	TestSmartPtr2()
	{
		++constructor_counter_;
		std::cout << __func__ <<" :"<< constructor_counter_ << std::endl;
	}
	~TestSmartPtr2()
	{
		++destructor_counter_;
		std::cout << __func__ << " :" << destructor_counter_ << std::endl;
	}

	static void clear()
	{
		constructor_counter_ = 0;
		destructor_counter_ = 0;
	}

protected:

	static int constructor_counter_;

	static int destructor_counter_;
};

int TestSmartPtr2::constructor_counter_ = 0;
int TestSmartPtr2::destructor_counter_ = 0;


int test_unique_ptr(int argc,char *argv[])
{
	std::unique_ptr<TestSmartPtr1> ptest(new TestSmartPtr1("123"));
	std::unique_ptr<TestSmartPtr1> ptest2(new TestSmartPtr1("456"));
	ptest->print();

	ptest2 = std::move(ptest);
	//不能直接ptest2 = ptest
	if (ptest == NULL)
	{
		std::cout << "ptest = NULL\n";
	}
		
	TestSmartPtr1* p = ptest2.release();
	p->print();
	ptest.reset(p);
	

	ptest->print();
	ptest2 = fun(); //这里可以用=，因为使用了移动构造函数
	ptest2->print();

	//两个的构造函数都只使用了1次。
	std::unique_ptr<TestSmartPtr2> p1(new TestSmartPtr2());
	TestSmartPtr2::clear();
	std::unique_ptr<TestSmartPtr2> p2 = std::make_unique<TestSmartPtr2>();


	//用于数组
	std::unique_ptr<int[]> abc(new int[1024]);

	memset(abc.get(), 0, 1024);

	auto p4 = abc.release();
	//必须删除abc
	delete p4;

	return 0;
}