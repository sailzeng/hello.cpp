
#include <iostream>     // std::cout
#include <future>       // std::packaged_task, std::future
#include <chrono>       // std::chrono::seconds
#include <thread>       // std::thread, std::this_thread::sleep_for

//==============================================================================
//future ������Ϊ�������ݸ���������������

void print_int_01(std::future<int>& fut) {
    int x = fut.get(); // ��ȡ����״̬��ֵ.
    std::cout << "value: " << x << '\n'; // ��ӡ value: 10.
}

int test_promise_01(int argc, char* argv[])
{
    std::promise<int> prom; // ����һ�� std::promise<int> ����.
    std::future<int> fut = prom.get_future(); // �� future ����.
    std::thread t(print_int_01, std::ref(fut)); // �� future ��������һ���߳�t.
    prom.set_value(10); // ���ù���״̬��ֵ, �˴����߳�t����ͬ��.
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

    global_prom = std::promise<int>();    // prom ��move��ֵΪһ���µ� promise ����.

    std::thread th2(print_global_promise);
    global_prom.set_value(20);
    th2.join();

    return 0;
}