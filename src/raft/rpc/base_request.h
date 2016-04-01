#ifndef BASE_REQUEST_H_ISDMXOF7
#define BASE_REQUEST_H_ISDMXOF7
#include <string>

namespace raft {

class BaseRequest {
public:
    BaseRequest() {}
    virtual ~BaseRequest() {}

    virtual void onFinish(bool ok) = 0;
};
    
} /* namespace raft  */ 


#endif /* end of include guard: BASE_REQUEST_H_ISDMXOF7 */
