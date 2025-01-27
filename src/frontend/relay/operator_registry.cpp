#include "mytvm/relay/operator_registry.h"

namespace mytvm {
namespace relay {

// Add the definition here
OperatorRegistry* OperatorRegistry::_operator_registry_instance = new OperatorRegistry();

std::vector<std::shared_ptr<RelayExpr>> _convert_gemm(onnx::NodeProto& node, const std::vector<std::shared_ptr<RelayExpr>>& inputs){
    std::vector<std::shared_ptr<RelayExpr>> outputs;
    if(inputs.size() != 3){
        throw std::runtime_error("Gemm operator expects 3 inputs");
    }
    // TODO: Implement the conversion function
    if(inputs.size() < 3)
        throw std::runtime_error("Gemm operator expects 3 inputs");

    std::shared_ptr<RelayOp> denseop = std::make_shared<RelayOp>("relay.nn.dense");
    std::vector<std::shared_ptr<RelayExpr>> denseargs;
    denseargs.push_back(inputs[0]);
    denseargs.push_back(inputs[1]);
    std::shared_ptr<RelayCall> dense = std::make_shared<RelayCall>(denseop, denseargs);

    std::shared_ptr<RelayOp> biasaddop = std::make_shared<RelayOp>("relay.nn.bias_add");
    std::vector<std::shared_ptr<RelayExpr>> biasaddargs;
    biasaddargs.push_back(dense);
    biasaddargs.push_back(inputs[2]);
    std::shared_ptr<RelayCall> biasadd = std::make_shared<RelayCall>(biasaddop, biasaddargs);

    outputs.push_back(biasadd);

    return outputs;
}

OperatorRegistry* OperatorRegistry::get_instance(){
    return _operator_registry_instance;
}

void OperatorRegistry::register_all_ops(){
    OperatorRegistry* registry = this;
    registry->registerOp("Gemm", _convert_gemm);
}

} // namespace relay
} // namespace mytvm
