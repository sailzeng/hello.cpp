
#include <iostream>     // std::cout
#include <future>       // std::packaged_task, std::future
#include <chrono>       // std::chrono::seconds
#include <thread>       // std::thread, std::this_thread::sleep_for

//==============================================================================
//future 用于作为参数传递给函数，当作参数

void print_int_01(std::future<int>& fut) {
    int x = fut.get(); // 获取共享状态的值.
    std::cout << "value: " << x << '\n'; // 打印 value: 10.
}

int test_promise_01(int argc, char* argv[])
{
    std::promise<int> prom; // 生成一个 std::promise<int> 对象.
    std::future<int> fut = prom.get_future(); // 和 future 关联.
    std::thread t(print_int_01, std::ref(fut)); // 将 future 交给另外一个线程t.
    prom.set_value(10); // 设置共享状态的值, 此处和线程t保持同步.
    t.join();
    return 0;
}

//==============================================================================

std::promise<int> global_prom;

void print_global_promise() {
    std::future<int> fut = global_prom.get_future();
    int x = fut.get();
    std::cout << "value: " << x << '\n';
}

int test_promise_02(int argc, char* argv[])
{
    std::thread th1(print_global_promise);
    global_prom.set_value(10);
    th1.join();

    global_prom = std::promise<int>();    // prom 被move赋值为一个新的 promise 对象.

    std::thread th2(print_global_promise);
    global_prom.set_value(20);
    th2.join();

    return 0;
}