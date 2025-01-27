#ifndef _MYTVM_RELAY_OPERATOR_REGISTRY_H_
#define _MYTVM_RELAY_OPERATOR_REGISTRY_H_

#include <functional>
#include <unordered_map>
#include <string>
#include <vector>

#include "onnx/onnx.pb.h"
#include "mytvm/relay/relay_include.h"

namespace mytvm {
namespace relay {

using ConversionFunc = std::function<std::vector<std::shared_ptr<RelayExpr>>(onnx::NodeProto&, const std::vector<std::shared_ptr<RelayExpr>>&)>;

class OperatorRegistry { 
private:
    std::unordered_map<std::string, ConversionFunc> registry;
    static OperatorRegistry* _operator_registry_instance;
    void register_all_ops();
public:
    OperatorRegistry() {
        register_all_ops(); 
    }

    static OperatorRegistry* get_instance();

    void registerOp(const std::string& optype, ConversionFunc func){
        registry[optype] = std::move(func);
    }

    ConversionFunc getConversionFunc(const std::string& optype){
        auto it = registry.find(optype);
        if(it != registry.end()){
            return it->second;
        }
        throw std::runtime_error("Operator not found in registry: " + optype);
    }

    std::vector<std::shared_ptr<RelayExpr>> convertNode(onnx::NodeProto& node, const std::vector<std::shared_ptr<RelayExpr>>& inputs){
        auto it = registry.find(node.op_type());
        if(it != registry.end()){
            return it->second(node, inputs);
        }
        throw std::runtime_error("Operator not found in registry: " + node.op_type());
    }
};



} // namespace relay
} // namespace mytvm

#endif // _MYTVM_RELAY_OPERATOR_REGISTRY_H_
