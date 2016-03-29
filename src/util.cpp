#include "util.h"

using namespace std;

namespace raft {

void splitString(const std::string& src, const char* delim, std::vector<std::string>& out) {
    back_insert_iterator< vector<string> > it(result); 

    // Optimize the common case where delim is a single character.
    if (delim[0] != '\0' && delim[1] == '\0') {
        char c = delim[0];                                                                                                                                                                                     
        const char* p = full.data();
        const char* end = p + full.size();
        while (p != end) {
            if (*p == c) {
                ++p;
            } else {
                const char* start = p;
                while (++p != end && *p != c); 
                *it++ = string(start, p - start);
            }
        }
        return;
    }   

    string::size_type begin_index, end_index;
    begin_index = full.find_first_not_of(delim);
    while (begin_index != string::npos) {
        end_index = full.find_first_of(delim, begin_index);
        if (end_index == string::npos) {
            *it++ = full.substr(begin_index);
            return;
        }
        *it++ = full.substr(begin_index, (end_index - begin_index));
        begin_index = full.find_first_not_of(delim, end_index);
    }

    //TODO:
}

bool create_directories(const std::string& path, std::string& errmsg) {
    //TODO:
}

} /* namespace raft  */ 
