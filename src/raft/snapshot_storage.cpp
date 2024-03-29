#include "snapshot_storage.h"

#include <iostream>
#include <boost/filesystem.hpp>

namespace raft {

SnapShotStorage::SnapShotStorage(const std::string& data_dir) 
    : snap_path_(data_dir + "/snap") 
{
    namespace fs = boost::filesystem;

    // 创建目录                                                                                                                                                                                                
    fs::path path(snap_path_);
    try {
        if (!fs::exists(path))
            fs::create_directories(path);
        else if (!fs::is_directory(path)) {
            auto ec = boost::system::errc::make_error_code(boost::system::errc::not_a_directory);
            throw fs::filesystem_error("already occupied", ec);
        }
    }
    catch (fs::filesystem_error& e) {
        std::cerr << "Create snap data directory failed.(" << e.what() << ")" << std::endl;
        exit(EXIT_FAILURE);
    }
}

SnapShotStorage::~SnapShotStorage() {
}

} /* namespace raft  */ 
