#ifndef _MYTVM_RELAY_OPERATOR_H_
#define _MYTVM_RELAY_OPERATOR_H_

#include "./relay_include.h"
#include <string>
#include <vector>

namespace mytvm {
namespace relay {

class RelayOperator : public RelayExpr {<Up>
public:
    std::string name;
    std::vector<std::shared_ptr<Type>> arg_types;

    RelayOperator(std::string& n, std::vector<std::shared_ptr<Type>>& at) {
        name = n;
        arg_types = std::move(at);
    }
    virtual ~RelayOperator() {}
    virtual void print() = 0;
};

} // namespace relay
} // namespace mytvm

#endif // _MYTVM_RELAY_OPERATOR_H_
