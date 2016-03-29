#ifndef UTIL_H_HFAWDXRS
#define UTIL_H_HFAWDXRS
#include <string>
#include <vector>

namespace raft {

void splitString(const std::string& src, const char* delim, std::vector<std::string>& out);

bool create_directories(const std::string& path, std::string& errmsg);


} /* namespace raft  */ 


#endif /* end of include guard: UTIL_H_HFAWDXRS */
