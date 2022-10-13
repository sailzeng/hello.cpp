// Hello.Cpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "main.h"

#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>

using namespace std::chrono;

int hello_chrono_duration(int argc, char* argv[])
{
    std::chrono::microseconds us1(3);
    std::chrono::microseconds us2(3000);

    std::chrono::seconds sec(3);

    auto us3 = us2 - us1;
    std::cout << "us3:" << us3.count() << std::endl;

    auto us4 = sec - us2;
    std::cout << "us4:" << us4.count() << std::endl;

    //好像是向下取整
    std::cout << "us 4 cast second" << std::chrono::duration_cast<std::chrono::seconds>(us4).count() << std::endl;

    std::chrono::duration<double, std::ratio<1, 30> > hz30(3.5);

    std::cout << "3.5 30hz:" << hz30.count() << std::endl;

    return 0;
}

int hello_chrono_timepoint(int argc, char* argv[])
{
    std::cout << "=============================================" << std::endl;

    std::chrono::system_clock::time_point t1 = std::chrono::system_clock::now();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::chrono::system_clock::time_point t2 = std::chrono::system_clock::now();
    auto t = t2 - t1;

    std::cout << "duration:" << t.count() << std::endl;

    typedef std::chrono::duration<long long, std::ratio<24 * 60 * 60, 1> > daystypoe;
    std::chrono::time_point<std::chrono::system_clock, daystypoe  > days;

    return 0;
}

int hello_chrono_clock(int argc, char* argv[])
{
    std::cout << "=============================================" << std::endl;

    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::cout << "system_clock now is:" << now.time_since_epoch().count() << std::endl;
    //不是秒，是毫秒，
    auto now1_data = now.time_since_epoch();
    std::cout << "system_clock duration type name " << typeid(now1_data).name() << std::endl;
    std::cout << std::fixed << std::setprecision(20);
    std::cout << "system_clock duration type precision:"
        << " " << decltype(now1_data)::period::num
        << " " << decltype(now1_data)::period::den
        << " " << double(decltype(now1_data)::period::num * 1.0) / decltype(now1_data)::period::den
        << std::endl;
    std::cout << "system_clock duration type precision:"
        << " " << std::chrono::system_clock::time_point::duration::period::num
        << " " << std::chrono::system_clock::time_point::duration::period::den
        << " " << double(std::chrono::system_clock::time_point::duration::period::num * 1.0) / std::chrono::system_clock::time_point::duration::period::den
        << std::endl;

    std::chrono::high_resolution_clock::time_point now2 = std::chrono::high_resolution_clock::now();
    std::cout << "high_resolution_clock now is:" << now2.time_since_epoch().count() << std::endl;
    auto now2_data = now2.time_since_epoch();
    std::cout << "high_resolution_clock duration type name " << typeid(now2_data).name() << std::endl;
    std::cout << "high_resolution_clock duration type precision:"
        << " " << decltype(now2_data)::period::num
        << " " << decltype(now2_data)::period::den
        << " " << double(decltype(now2_data)::period::num * 1.0) / decltype(now2_data)::period::den
        << std::endl;

    std::chrono::steady_clock::time_point now3 = std::chrono::steady_clock::now();
    std::cout << "steady_clock now is:" << now3.time_since_epoch().count() << std::endl;
    auto now3_data = now3.time_since_epoch();
    std::cout << "" << typeid(now3_data).name() << std::endl;
    std::cout << "steady_clock duration type precision:"
        << " " << decltype(now3_data)::period::num
        << " " << decltype(now3_data)::period::den
        << " " << double(decltype(now3_data)::period::num * 1.0) / decltype(now3_data)::period::den
        << std::endl;

    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    tm now_tm = *std::localtime(&now_time_t);
    std::cout << "Now ,the time was " <<
        std::put_time(&now_tm, "%F %T") << std::endl;

    return 0;
}

int test_chrono_literals()
{
    using namespace std::chrono_literals;
    auto d1 = 250ms;
    std::chrono::milliseconds d2 = 1ms;
    std::cout << "250ms = " << d1.count() << " milliseconds\n"
        << "1s = " << d2.count() << " milliseconds\n";
    return 0;
}

class ZCE_Chrono_HR_Timer
{
public:

    ///构造函数
    ZCE_Chrono_HR_Timer() :
        addup_time_(std::chrono::high_resolution_clock::duration::zero())
    {
    }
    ///析构函数
    ~ZCE_Chrono_HR_Timer() = default;

    ///从新开始计时
    void restart()
    {
        start_time_ = std::chrono::high_resolution_clock::now();
        addup_time_ = std::chrono::high_resolution_clock::duration::zero();
    }
    ///结束计时
    void end()
    {
        end_time_ = std::chrono::high_resolution_clock::now();
    }
    ///累计计时开始,用于多次计时的过程，
    void addup_start()
    {
        if (end_time_ > start_time_)
        {
            addup_time_ += end_time_ - start_time_;
        }

        start_time_ = std::chrono::high_resolution_clock::now();
    }

    ///计算消耗的时间(us,微妙 -6)
    double elapsed_usec() const
    {
        const double NSEC_PER_USEC = 1000.0;
        if (end_time_ > start_time_)
        {
            return std::chrono::duration_cast<std::chrono::nanoseconds>((end_time_ - start_time_) + addup_time_).count() /
                NSEC_PER_USEC;
        }
        else
        {
            return  0.0f;
        }
    }

    ///精度
    static double precision_usec()
    {
        const double USEC_PER_SEC = 1000000.0;
        return double(std::chrono::high_resolution_clock::time_point::duration::period::num * USEC_PER_SEC) /
            double(std::chrono::high_resolution_clock::time_point::duration::period::den);
    }

protected:

    ///开始的时间
    std::chrono::high_resolution_clock::time_point start_time_;
    ///结束的时间
    std::chrono::high_resolution_clock::time_point end_time_;

    ///累计时间
    std::chrono::high_resolution_clock::duration addup_time_;
};

int hello_progress_timer(int argc, char* argv[])
{
    ZCE_Chrono_HR_Timer hr_timer;
    std::cout << "=============================================" << std::endl;

    hr_timer.restart();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    hr_timer.end();

    std::cout << "duration:" << hr_timer.elapsed_usec() << std::endl;

    return 0;
}