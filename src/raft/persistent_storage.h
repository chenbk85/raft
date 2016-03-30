#ifndef PERSISTENT_STORAGE_H_F3SHEXG9
#define PERSISTENT_STORAGE_H_F3SHEXG9
#include <leveldb/db.h>

namespace raft {

class PersistentStorage{
public:
    PersistentStorage(const std::string& dir);
    ~PersistentStorage();

private:
    const std::string db_path_;
    leveldb::DB *db_;
};
    
} /* namespace raft  */ 

#endif /* end of include guard: PERSISTENT_STORAGE_H_F3SHEXG9 */
