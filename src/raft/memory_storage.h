#ifndef MEMORY_STORAGE_H_8J26PHC3
#define MEMORY_STORAGE_H_8J26PHC3

namespace raft {

class MemoryStorage {
public:
    MemoryStorage();
    ~MemoryStorage();

private:
    uint64_t commit_index_;     // 已提交的日志最高index
    uint64_t applied_index_;    // 已应用到状态机的日志最高index
};
    
} /* namespace raft  */ 


#endif /* end of include guard: MEMORY_STORAGE_H_8J26PHC3 */
