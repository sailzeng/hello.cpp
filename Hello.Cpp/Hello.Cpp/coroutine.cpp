#include <coroutine>
#include <iostream>
#include <stdexcept>
#include <thread>


struct test
{
    // inner types
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>; //type alias

                                                                           // functions
    test(handle_type h) :handle_(h) {
        std::cout << "# Created a Test object\n";
    }
    test(const test& s) = delete;
    test& operator=(const test&) = delete;
    test(test&& s) :handle_(s.handle_) {
        s.handle_ = nullptr;
    }
    test& operator=(test&& s) {
        handle_ = s.handle_; s.handle_ = nullptr; return *this;
    }
    ~test() {
        std::cout << "#Test gone\n"; if (handle_) handle_.destroy();
    }

    int current_value()
    {
        return handle_.promise().value_;
    }

    bool move_next()
    {
        handle_.resume();
        return !handle_.done();
    }

    struct promise_type
    {
        promise_type()
        {
            std::cout << "@ promise_type created\n";
        }
        ~promise_type()
        {
            std::cout << "@ promise_type died\n";
        }

        auto get_return_object() //get return object
        {
            std::cout << "@ get_return_object called\n";
            return test{ handle_type::from_promise(*this) };// pass handle to create "return object"
        }

        auto initial_suspend() // called before run coroutine body
        {
            std::cout << "@ initial_suspend is called\n";
            return std::suspend_never{}; // dont suspend it
            //return std::suspend_always{};
        }

        auto return_void() // called when just before final_suspend, conflict with return_value
        {
            std::cout << "@ return_void is called\n";
            return; // dont suspend it
            //return std::experimental::suspend_always{};
        }

        auto yield_value(int t) // called by co_yield()
        {
            std::cout << "yield_value called\n";
            value_ = t;
            return std::suspend_always{};
        }

        auto final_suspend() noexcept// called at the end of coroutine body
        {
            std::cout << "@ final_suspend is called\n";
            return std::suspend_always{};
        }

        void unhandled_exception() // exception handler
        {
            std::exit(1);
        }

        // data
        int value_;
    };

    // member variables
    handle_type handle_;
};
test yield_coroutine(int count)
{
    std::cout << "start yield_coroutine\n";
    for (int i = 0; i < count; i++)
        co_yield i * 2;
    //注意这儿并没有return
}

void co_vs_yield()
{
    auto a = yield_coroutine(4);
    std::cout << "created a corutine, try to get a value\n";
    do
    {
        std::cout << "get value " << a.current_value() << std::endl;
    } while (a.move_next());
}

int test_coro_main2(int argc, char* argv[])
{
    co_vs_yield();
    return 0;
}

// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


size_t level = 0;
std::string INDENT = "-";

class Trace
{
public:
    Trace()
    {
        in_level();
    }
    ~Trace()
    {
        level -= 1;
    }
    void in_level()
    {
        level += 1;
        std::string res(INDENT);
        for (size_t i = 0; i < level; i++)
        {
            res.append(INDENT);
        };
        std::cout << res;
    }
};

template <typename T>
struct sync
{
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;
    handle_type coro;

    sync(handle_type h)
        : coro(h)
    {
        Trace t;
        std::cout << "Created a sync object" << std::endl;
    }
    sync(const sync&) = delete;
    sync(sync&& s)
        : coro(s.coro)
    {
        Trace t;
        std::cout << "Sync moved leaving behind a husk" << std::endl;
        s.coro = nullptr;
    }
    ~sync()
    {
        Trace t;
        std::cout << "Sync gone" << std::endl;
        if (coro)
            coro.destroy();
    }
    sync& operator=(const sync&) = delete;
    sync& operator=(sync&& s)
    {
        coro = s.coro;
        s.coro = nullptr;
        return *this;
    }

    T get()
    {
        Trace t;
        std::cout << "We got asked for the return value..." << std::endl;
        return coro.promise().value_;
    }
    struct promise_type
    {
        T value_;
        promise_type()
        {
            Trace t;
            std::cout << "Promise created" << std::endl;
        }
        ~promise_type()
        {
            Trace t;
            std::cout << "Promise died" << std::endl;
        }

        auto get_return_object()
        {
            Trace t;
            std::cout << "Send back a sync" << std::endl;
            return sync<T>{handle_type::from_promise(*this)};
        }
        auto initial_suspend()
        {
            Trace t;
            std::cout << "Started the coroutine, don't stop now!" << std::endl;
            return std::suspend_never{};
            //std::cout << "--->Started the coroutine, put the brakes on!" << std::endl;
            //return std::experimental::suspend_always{};
        }
        void return_value(T v)
        {
            Trace t;
            std::cout << "Got an answer of " << v << std::endl;
            value_ = v;
            return;
        }
        auto final_suspend() noexcept
        {
            Trace t;
            std::cout << "Finished the coro" << std::endl;
            return std::suspend_always{};
        }
        void unhandled_exception()
        {
            std::exit(1);
        }
    };
};

template <typename T>
struct lazy
{
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;
    handle_type coro_;

    lazy(handle_type h)
        : coro_(h)
    {
        Trace t;
        std::cout << "Created a lazy object" << std::endl;
    }
    lazy(const lazy&) = delete;
    lazy(lazy&& s)
        : coro_(s.coro_)
    {
        Trace t;
        std::cout << "lazy moved leaving behind a husk" << std::endl;
        s.coro_ = nullptr;
    }
    ~lazy()
    {
        Trace t;
        std::cout << "lazy gone" << std::endl;
        if (coro_)
            coro_.destroy();
    }
    lazy& operator=(const lazy&) = delete;
    lazy& operator=(lazy&& s)
    {
        coro_ = s.coro_;
        s.coro_ = nullptr;
        return *this;
    }

    T get()
    {
        Trace t;
        std::cout << "We got asked for the return value..." << std::endl;
        return coro_.promise().value_;
    }
    struct promise_type
    {
        T value_;
        promise_type()
        {
            Trace t;
            std::cout << "Promise created" << std::endl;
        }
        ~promise_type()
        {
            Trace t;
            std::cout << "Promise died" << std::endl;
        }

        auto get_return_object()
        {
            Trace t;
            std::cout << "Send back a lazy" << std::endl;
            return lazy<T>{handle_type::from_promise(*this)};
        }
        auto initial_suspend()
        {
            Trace t;
            //std::cout << "Started the coroutine, don't stop now!" << std::endl;
            //return std::experimental::suspend_never{};
            std::cout << "Started the coroutine, put the brakes on!" << std::endl;
            return std::suspend_always{};
        }
        void return_value(T v)
        {
            Trace t;
            std::cout << "Got an answer of " << v << std::endl;
            value_ = v;
            return;
        }
        auto yield_value(T v)
        {
            Trace t;
            std::cout << "Got yeild answer of " << v << std::endl;
            value_ = v;
            return std::suspend_always{};
        }
        auto final_suspend() noexcept
        {
            Trace t;
            std::cout << "Finished the coro" << std::endl;
            return std::suspend_always{};
        }
        void unhandled_exception()
        {
            std::exit(1);
        }

    };
    bool await_ready()
    {
        const auto ready = this->coro_.done();
        Trace t;
        std::cout << "Await " << (ready ? "is ready" : "isn't ready") << std::endl;
        return this->coro_.done();
    }
    void await_suspend(std::coroutine_handle<> awaiting)
    {
        {
            Trace t;
            std::cout << "About to resume the lazy" << std::endl;
            this->coro_.resume();
        }
        Trace t;
        std::cout << "About to resume the awaiter" << std::endl;
        awaiting.resume();
    }
    auto await_resume()
    {
        const auto r = this->coro_.promise().value_;
        Trace t;
        std::cout << "Await value is returned: " << r << std::endl;
        return r;
    }
};
lazy<std::string> read_data()
{
    Trace t;
    std::cout << "Reading data..." << std::endl;
    //co_yield "million$!?";

    co_return "billion$!";
}

lazy<std::string> write_data()
{
    Trace t;
    std::cout << "Write data..." << std::endl;
    co_return "I'm rich!";
}

sync<int> reply()
{
    std::cout << "Started await_answer" << std::endl;
    auto a = co_await read_data();
    std::cout << "Data we got is " << a << std::endl;
    auto v = co_await write_data();
    std::cout << "write result is " << v << std::endl;
    co_return 42;
}



int test_coro_main(int argc, char* argv[])
{
    std::cout << "Start main()\n";
    //没有使用co_wait调用。返回的是一个sync<int>
    auto a = reply();
    std::cout << "get value " << a.get() << std::endl;
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

template <typename T>
struct coro_ret
{
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;
    handle_type coro_handle_;

    coro_ret(handle_type h)
        : coro_handle_(h)
    {
    }
    coro_ret(const coro_ret&) = delete;
    coro_ret(coro_ret&& s)
        : coro_handle_(s.coro_)
    {
        s.coro_handle_ = nullptr;
    }
    ~coro_ret()
    {
        if (coro_handle_)
            coro_handle_.destroy();
    }
    coro_ret& operator=(const coro_ret&) = delete;
    coro_ret& operator=(coro_ret&& s)
    {
        coro_handle_ = s.coro_handle_;
        s.coro_handle_ = nullptr;
        return *this;
    }

    bool move_next()
    {
        coro_handle_.resume();
        return coro_handle_.done();
    }

    T get()
    {
        return coro_handle_.promise().return_data_;
    }

    struct promise_type
    {
        promise_type() = default;
        ~promise_type() = default;

        auto get_return_object()
        {
            return coro_ret<T>{handle_type::from_promise(*this)};
        }
        //注意这个函数,如果返回std::suspend_never{}，就不挂起，
        //返回std::suspend_always{} 挂起
        auto initial_suspend()
        {
            //return std::suspend_never{};
            return std::suspend_always{};
        }
        //co_return 后这个函数会被调用
        void return_value(T v)
        {
            return_data_ = v;
            return;
        }
        auto yield_value(T v)
        {
            std::cout << "yield_value invoked." << std::endl;
            return_data_ = v;
            return std::suspend_always{};
        }
        //若 final_suspend 返回 std::suspend_always 则需要用户自行调用 handle.destroy() 进行销毁
        auto final_suspend() noexcept
        {
            std::cout << "final_suspend invoked." << std::endl;
            return std::suspend_always{};
        }
        void unhandled_exception()
        {
            std::exit(1);
        }
        //返回值
        T return_data_;
    };
};

//https://lewissbaker.github.io/2017/11/17/understanding-operator-co-await

template <typename P>
struct await_aio
{
    await_aio()
    {

    }

    bool await_ready()
    {
        return false;
    }
    void await_suspend(std::coroutine_handle<P> awaiting)
    {
        std::cout << "await_suspend" << std::endl;
        awaiting_ = awaiting;
    }
    void await_resume()
    {
        return;
    }

    //
    std::coroutine_handle<P> awaiting_;
};

coro_ret<int> yield_coroutine_count()
{
    std::cout << "Coroutine co_await std::suspend_never" << std::endl;
    co_await std::suspend_never{};
    std::cout << "Coroutine co_await std::suspend_always" << std::endl;
    co_await std::suspend_always{};
    std::cout << "Coroutine stage 1 ,co_yield" << std::endl;
    co_yield 101;
    std::cout << "Coroutine stage 2 ,co_yield" << std::endl;
    co_yield 202;
    std::cout << "Coroutine stage 3 ,co_yield" << std::endl;
    co_yield 303;
    std::cout << "Coroutine stage end, co_return" << std::endl;
    co_return 808;
}

int test_coro_main3(int argc, char* argv[])
{
    bool done = false;
    std::cout << "Start main()\n";
    auto c_r = yield_coroutine_count();
    //第一次停止因为initial_suspend 返回的是suspend_always
    //此时没有进入Stage 1
    std::cout << "Coroutine " << (done ? "is done " : "isn't done ") << "ret =" << c_r.get() << std::endl;
    done = c_r.move_next();
    //此时是，co_await std::suspend_always{}
    std::cout << "Coroutine " << (done ? "is done " : "isn't done ") << "ret =" << c_r.get() << std::endl;
    done = c_r.move_next();
    //此时打印Stage 1
    std::cout << "Coroutine " << (done ? "is done " : "isn't done ") << "ret =" << c_r.get() << std::endl;
    done = c_r.move_next();
    std::cout << "Coroutine " << (done ? "is done " : "isn't done ") << "ret =" << c_r.get() << std::endl;
    done = c_r.move_next();
    std::cout << "Coroutine " << (done ? "is done " : "isn't done ") << "ret =" << c_r.get() << std::endl;
    done = c_r.move_next();
    std::cout << "Coroutine " << (done ? "is done " : "isn't done ") << "ret =" << c_r.get() << std::endl;
    return 0;
}


