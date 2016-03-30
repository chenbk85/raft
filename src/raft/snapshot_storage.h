#ifndef SNAPSHOT_STORAGE_H_GMCYWBTZ
#define SNAPSHOT_STORAGE_H_GMCYWBTZ
#include <string>

namespace raft {

class SnapShotStorage {
public:
    SnapShotStorage(const std::string& data_dir);
    ~SnapShotStorage();

private:
    const std::string snap_path_;
};
    
} /* namespace raft  */ 



#endif /* end of include guard: SNAPSHOT_STORAGE_H_GMCYWBTZ */
