#ifndef STABLE_STORAGE_H_HZMNIGDX
#define STABLE_STORAGE_H_HZMNIGDX
#include <leveldb/db.h>

namespace raft {

class StableStorage {
public:
    StableStorage(const std::string& dir);
    ~StableStorage();

private:
    leveldb::DB *db_;
};
    
} /* namespace raft  */ 

#endif /* end of include guard: STABLE_STORAGE_H_HZMNIGDX */
