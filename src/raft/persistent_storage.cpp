#include "persistent_storage.h"

#include <iostream>
#include <boost/filesystem.hpp>

namespace raft {

PersistentStorage::PersistentStorage(const std::string& dir):
    db_path_(dir + "/entry"),
    db_(NULL) 
{
    namespace fs = boost::filesystem;
    
    // 创建目录                                                                                                                                                                                                
    fs::path path(db_path_);
    try {
        if (!fs::exists(path))
            fs::create_directories(path);
        else if (!fs::is_directory(path)) {
            auto ec = boost::system::errc::make_error_code(boost::system::errc::not_a_directory);
            throw fs::filesystem_error("already occupied", ec);
        }
    }
    catch (fs::filesystem_error& e) {
        std::cerr << "Create entry data directory failed.(" << e.what() << ")" << std::endl;
        exit(EXIT_FAILURE);
    }

    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb", &db_);
    if (!status.ok()) {
        std::cerr << "Open leveldb failed. (" << status.ToString() << ")" << std::endl;
        exit(EXIT_FAILURE);
    }
}

PersistentStorage::~PersistentStorage() {
}
    
} /* namespace raft  */ 
