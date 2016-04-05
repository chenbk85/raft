#ifndef TIMER_H_OYHJACPD
#define TIMER_H_OYHJACPD
#include <memory>
#include <chrono>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>

namespace raft {

class Timeoutable {
public:
    Timeoutable () {}
    virtual ~Timeoutable () {}

    virtual void onTimeout() = 0;
};


class TimeoutQueue {
public:
    TimeoutQueue ();
    ~TimeoutQueue ();

    void push(std::shared_ptr<Timeoutable> t, int timeout_msec);

private:
    void dispatch();

private:
    struct Timer {
        std::weak_ptr<Timeoutable> wp_entity;
        std::chrono::time_point<std::chrono::steady_clock> deadline;

        bool operator>(const Timer& rhs) const {
            return deadline > rhs.deadline;
        }
    };

    std::priority_queue<Timer, std::vector<Timer>, std::greater<Timer> > q_;
    std::mutex mu_;
    std::condition_variable cv_;
    bool exit_;
    std::thread dispatch_thr_;
};


} /* namespace raft  */ 

#endif /* end of include guard: TIMER_H_OYHJACPD */
