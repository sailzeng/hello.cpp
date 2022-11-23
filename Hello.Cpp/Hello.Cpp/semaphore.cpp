#include <iostream>
#include <semaphore>
#include <thread>

constexpr int countlimit = 10000;

std::counting_semaphore<0xFFFF> signal2ping(countlimit);       // (1)
std::counting_semaphore<0xFFFF> signal2pong(0);   // (2)

std::atomic<int> counter{ 0 };

void ping()
{
    bool ret = false;
    while (counter < countlimit)
    {
        ret = signal2ping.try_acquire();                  // (5)
        if (!ret)
        {
            continue;
        }
        ++counter;
        signal2pong.release();
    }
    std::cout << "ping" << std::endl;
}

void pong()
{
    bool ret = false;
    while (counter < countlimit)
    {
        ret = signal2pong.try_acquire();
        if (!ret)
        {
            continue;
        }
        --counter;
        signal2ping.release();                  // (3)
    }
    std::cout << "pong" << std::endl;
}

int hello_semaphore(int argc, char* argv[])
{
    auto start = std::chrono::system_clock::now();

    std::thread t1(ping);
    std::thread t2(pong);

    t1.join();
    t2.join();

    std::chrono::duration<double> dur = std::chrono::system_clock::now() - start;
    std::cout << "Duration: " << dur.count() << " seconds" << '\n';
    std::cout << "signal2ping " << signal2ping.max() << std::endl;
    std::cout << "std::counting_semaphore<0xFFFF>::max " << std::counting_semaphore<0xFFFF>::max() << std::endl;
    return 0;
}