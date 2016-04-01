#ifndef TIMER_H_OYHJACPD
#define TIMER_H_OYHJACPD
#include <memory>
#include <chrono>
#include <queue>
#include <stdio.h>

namespace raft {

class Timeoutable {
public:
    Timer() {}
    virtual ~Timer() {}

    virtual void onTimeout() = 0;
};


class TimerDispatcher {
public:
    TimerDispatcher();
    ~TimerDispatcher();

    void push(const std::shared_ptr<Timeoutable>& t, int timeout_msec);

private:
    struct Timer {
        std::weak_ptr<Timeoutable> wp_entity;
        std::chrono::time_point<std::chrono::steady_clock> deadline;
    };

    //TODO:
};


} /* namespace raft  */ 

#endif /* end of include guard: TIMER_H_OYHJACPD */
