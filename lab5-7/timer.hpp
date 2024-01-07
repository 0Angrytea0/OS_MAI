#pragma once
#include <chrono>

class Timer
{
public:
    Timer() = default;
    ~Timer() = default;

    void start_timer()
    {
        is_timer_started = true;
        start_ = std::chrono::steady_clock::now();
    }
    void stop_timer(){
        if (is_timer_started)
        {
            is_timer_started = false;
            finish_ = std::chrono::steady_clock::now();
        }
    }
    int get_time()
    {
        if (is_timer_started)
        {
            finish_ = std::chrono::steady_clock::now();
        }
        return std::chrono::duration_cast<std::chrono::milliseconds>(finish_ - start_).count();
    }

private:
    bool is_timer_started = false;
    std::chrono::steady_clock::time_point start_;
    std::chrono::steady_clock::time_point finish_;
};

