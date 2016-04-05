#include "util.h"
#include <random>

namespace raft {

int randomInt(int min, int max) {
    thread_local std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

} /* namespace raft  */ 
