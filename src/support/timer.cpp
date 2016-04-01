#include "timer.h"

namespace raft {

TimerDispatcher::TimerDispatcher() {
}

TimerDispatcher::~TimerDispatcher() {
}

void TimerDispatcher::push(const std::shared_ptr<Timeoutable>& t, int timeout_msec) {
}

} /* namespace raft  */ 
