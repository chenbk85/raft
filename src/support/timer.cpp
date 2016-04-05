#include "timer.h"
#include <iostream>
#include <functional>

namespace raft {

TimeoutQueue::TimeoutQueue() :
    exit_(false),
    dispatch_thr_(std::bind(&TimeoutQueue::dispatch, this))
{
}

TimeoutQueue::~TimeoutQueue() {
    {
        std::lock_guard<std::mutex> lock(mu_);
        exit_ = true;
    }
    cv_.notify_one();
    dispatch_thr_.join();
}

void TimeoutQueue::push(const std::shared_ptr<Timeoutable>& t, int timeout_msec) {
    Timer timer{t, std::chrono::steady_clock::now() + std::chrono::milliseconds(timeout_msec) };
    bool need_notify = false;

    {
        std::lock_guard<std::mutex> lock(mu_);
        q_.push(timer);
        need_notify = q_.top().deadline == timer.deadline;
    }

    if (need_notify)
        cv_.notify_one();
}


void TimeoutQueue::dispatch() {
    for ( ; ; ) {
        std::vector<Timer> timeouts;

        // get timeout timers
        {
            std::unique_lock<std::mutex> lock(mu_);

            if (exit_) {
                std::cout << "TimeoutQueue is exiting.." << std::endl;
                return;
            }

            auto timeout_time = q_.empty() ?
                std::chrono::steady_clock::now() + std::chrono::minutes(5) :
                q_.top().deadline;

            cv_.wait_until(lock, timeout_time);

            while (!q_.empty()) {
                auto& t = q_.top();
                if ( std::chrono::steady_clock::now() >= t.deadline) {
                    timeouts.push_back(t);
                    q_.pop();
                }
                else {
                    break;
                }
            }
        }

        // handle timeouts
        for (auto& t: timeouts) {
            auto sp = t.wp_entity.lock();
            if (sp) {
                try {
                    sp->onTimeout();
                }
                catch (...) {
                }
            }
        }

    }
}




} /* namespace raft  */ 
