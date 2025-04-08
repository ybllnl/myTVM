#ifndef _MYTVM_RELAY_MODULE_H_
#define _MYTVM_RELAY_MODULE_H_

#include <unordered_map>
#include <string>
#include "./relay_include.h"

namespace mytvm {

using namespace mytvm::relay;
class IRModule {
    public:
    /*! \brief The functions in the module, keyed by function name */
    std::unordered_map<std::string, std::shared_ptr<RelayFunction>> functions;

    IRModule() {}
};

}


#endif // _MYTVM_RELAY_MODULE_H_
