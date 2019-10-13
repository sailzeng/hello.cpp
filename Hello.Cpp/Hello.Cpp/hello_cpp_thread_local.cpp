#include <iostream>
#include <string>
#include <thread>
#include <mutex>



thread_local unsigned int rage=1;
std::mutex cout_mutex;

void increase_rage(const std::string& thread_name)
{
    for(int i=0;i<9000;++i)
    {
        ++rage;
        // 在锁外修改 OK；

        //锁是为了打印
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout<<"Rage counter for "<<thread_name<<": "<<rage<<'\n';
    }
    
}

int test_thread_local(int argc,char* argv[])
{
    std::thread a(increase_rage,"a"),b(increase_rage,"b");

    {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout<<"Rage counter for main: "<<rage<<'\n';
    }

    a.join();
    b.join();
    return 0;
}